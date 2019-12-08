package userApplication;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.Socket;
import java.net.UnknownHostException;
import javax.sound.sampled.*;
import java.util.*;
import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;

public class userApplication {

	int clientPort = 48010;
	int serverPort = 38010;

	public static void main(String[] args)
			throws SocketException, IOException, UnknownHostException, LineUnavailableException {

		userApplication self = new userApplication();
		// sess1
		/*
		 * byte[] copterCode = "Q0190".getBytes(); byte[] echoCode = "E3081".getBytes();
		 * byte[] echoTempCode = "E3081T00".getBytes(); byte[] imgCodePTZ =
		 * ("M4495FLOW=ON" + "UDP=1024" + "CAM=PTZ").getBytes(); byte[] imgCodeFIX =
		 * ("M4495FLOW=ON" + "UDP=1024" + "CAM=FIX").getBytes(); int audioPackets = 999;
		 * byte[] audioCodeAQ = ("A3776AQL15F" + audioPackets).getBytes(); byte[]
		 * audioCodeAQ1 = ("A3776AQL32F" + audioPackets).getBytes(); byte[]
		 * audioCodeSong = ("A3776L20F" + audioPackets).getBytes(); byte[] audioCodeFreq
		 * = ("A3776T" + audioPackets).getBytes();
		 */
		// session 2
		byte[] copterCode = "Q2996".getBytes();
		byte[] echoCode = "E4371".getBytes();
		byte[] echoTempCode = "E4371T00".getBytes();
		byte[] imgCodePTZ = ("M7257FLOW=ON" + "UDP=1024" + "CAM=PTZ").getBytes();
		byte[] imgCodeFIX = ("M7257FLOW=ON" + "UDP=1024" + "CAM=FIX").getBytes();
		int audioPackets = 999;
		byte[] audioCodeAQ = ("A0430AQL19F" + audioPackets).getBytes();
		byte[] audioCodeAQ1 = ("A0430AQL24F" + audioPackets).getBytes();
		byte[] audioCodeSong = ("A0430L15F" + audioPackets).getBytes();
		byte[] audioCodeFreq = ("A0430T" + audioPackets).getBytes();

		// ================= SESSION =================
		self.ithakiCopter(copterCode);
		self.echoPackets(echoCode, "samples/session2/echoResponse.txt");
		self.echoPackets(("E0000").getBytes(), "samples/session2/echo0000.txt");
		self.throughput(8, "samples/session2/echoResponse.txt", "samples/session2/throughputEcho.txt");
		self.throughput(8, "samples/session2/echo0000.txt", "samples/session2/throughput0000.txt");
		self.cameraImage(imgCodePTZ, "samples/session2/E1PTZ.jpg");
		self.cameraImage(imgCodeFIX, "samples/session2/E2FIX.jpg");
		self.getAudio(audioCodeSong, audioPackets, 8000, "DPCM", 0, false);
		self.getAudio(audioCodeFreq, audioPackets, 8000, "DPCM", 0, true);
		self.getAudio(audioCodeAQ, audioPackets, 8000, "AQ-DPCM", 0, false);
		self.getAudio(audioCodeAQ1, audioPackets, 8000, "AQ-DPCM", 1, false);

		// OBD

		String[] pid = { "1F", "0F", "11", "0C", "0D", "05" };

		String[] OBDfiles = { "samples/session2/OBDengineruntime.txt", "samples/session2/OBDairtemp.txt",
				"samples/session2/OBDthrottlepos.txt", "samples/session2/OBDrpm.txt", "samples/session2/OBDspeed.txt",
				"samples/session2/OBDcooltemp.txt" };
		String OBDcode;
		for (int i = 0; i < pid.length; i++) {
			// OBDcode = "V3290" + "OBD=" + "01" + " " + pid[i];
			OBDcode = "V5058" + "OBD=" + "01" + " " + pid[i];
			self.vehicleOBDiagnostics(OBDcode.getBytes(), pid[i], OBDfiles[i]);
		}
		self.echoPackets(echoTempCode, "samples/session2/echoTemp.txt");

	}

	public void sendPacket(byte[] txbuffer, int port) {

		DatagramSocket sock = null;

		try {
			sock = new DatagramSocket();
		} catch (SocketException e) {
			System.out.println(e);
		}

		byte[] serverIP = { (byte) 155, (byte) 207, (byte) 18, (byte) 208 }; // Ithaki IP

		InetAddress serverAddress = null;

		try {
			serverAddress = InetAddress.getByAddress(serverIP);
		} catch (UnknownHostException e) {
			System.out.println(e);
		}

		DatagramPacket packet = new DatagramPacket(txbuffer, txbuffer.length);
		packet.setAddress(serverAddress);
		packet.setPort(port);

		try {
			sock.send(packet);
		} catch (IOException e) {
			System.out.println(e);
		}
		sock.close();
	}

	public void echoPackets(byte[] echoCode, String filename) {

		long responseTime;
		ArrayList<Long> responseTimes = new ArrayList<Long>();

		DatagramSocket r = null;

		try {
			r = new DatagramSocket(clientPort);
			r.setSoTimeout(2500); // 2500 to get no timeouts before packet
		} catch (SocketException e) {
			System.out.println(e);
		}

		byte[] rxbuffer = new byte[60]; // 32 bytes echo packet
		DatagramPacket q = new DatagramPacket(rxbuffer, rxbuffer.length);
		String message = null;

		long startTime = System.currentTimeMillis();
		int interval = 5 * 60000; // 5 λεπτά λειτουργίασ
		long endTime = startTime + interval;

		while (System.currentTimeMillis() < endTime) {

			sendPacket(echoCode, serverPort);

			long packetStart = System.currentTimeMillis();
			long packetEnd;

			try {
				r.receive(q);
				packetEnd = System.currentTimeMillis();
				message = new String(rxbuffer, 0, q.getLength());
				System.out.println(message);

				responseTime = packetEnd - packetStart; // Response = διαφορα των χρόνων
				responseTimes.add(responseTime);
			} catch (IOException e) {
				System.out.println(e);
			}

		}
		System.out.println(responseTimes);

		try {

			PrintWriter writer = new PrintWriter(filename, "UTF-8");
			for (long response : responseTimes) {
				writer.println(response);

			}

			writer.close(); // Κλείσιμο αρχείου

		} catch (Exception x) {
			;
		}

		r.close();
	}

	public void throughput(int interval, String readfile, String writefile) throws IOException { // interval for 8,16 or
																									// 32

		ArrayList<String> responseTimesStr = new ArrayList<String>(Files.readAllLines(Paths.get(readfile))); // new
																												// ArrayList<Long>();
		ArrayList<Long> responseTimes = new ArrayList<Long>();
		for (String k : responseTimesStr) {

			responseTimes.add(Long.parseLong(k, 10));
			System.out.println(Long.parseLong(k, 10));

		}

		ArrayList<Float> throughputs = new ArrayList<Float>();

		ArrayList<Long> summation = new ArrayList<Long>();
		long sum = 0;
		for (int i = 0; i < responseTimes.size(); i++) {

			for (int j = 0; j <= i; j++) {
				// long arr[] = {800,1200,320,4500,3500,1300,2700,700,1600,2400,900,1900};
				sum += responseTimes.get(j);

			}
			summation.add(sum);
			sum = 0;

		}

		float packetspersecond = 0;
		long lowerlimit = 0;
		long upperlimit = 1000 * interval;

		while (upperlimit < summation.get(summation.size() - 1) + 1000) {// for(int i = 0; i < summation.size(); i++) {

			for (int j = 0; j < summation.size(); j++) {

				if ((summation.get(j) > lowerlimit) && (summation.get(j) < upperlimit)) {
					System.out.println("I took " + summation.get(j) + " with lowerlimit " + lowerlimit
							+ " and upperlimit " + upperlimit);
					packetspersecond++;

				}

				// 800,2000,2320,6820,10320,11620,14320,15020,16620,19020,19920,21820,

			}

			lowerlimit += 1000;
			upperlimit += 1000;
			throughputs.add(packetspersecond / interval);
			packetspersecond = 0;

		}

		try {

			PrintWriter writer = new PrintWriter(writefile, "UTF-8");
			for (long i : summation) {
				// writer.println(thput);
				System.out.print(i + ",");

			}

			System.out.println();
			for (float thput : throughputs) {
				writer.println(thput);
				System.out.println(thput);

			}
			writer.close(); // Κλείσιμο αρχείου

		} catch (Exception x) {
			;
		}

	}

	public void cameraImage(byte[] imgCode, String imgFile) {

		DatagramSocket r = null;

		try {
			r = new DatagramSocket(clientPort);
			r.setSoTimeout(2500); // 2500 to get no timeouts before packet
		} catch (SocketException e) {
			System.out.println(e);
		}

		byte[] rxbuffer = new byte[1024];
		DatagramPacket q = new DatagramPacket(rxbuffer, rxbuffer.length);
		FileOutputStream image = null;
		try {
			image = new FileOutputStream(imgFile);
		} catch (FileNotFoundException e) {
			;
		}

		boolean endflag = false;
		// boolean startflag = false;

		while (!endflag) {

			/*
			 * while(!startflag) {
			 * 
			 * sendPacket(imgCode); try {
			 * 
			 * int start = 0; r.receive(q); for (int i = 0 ; i < rxbuffer.length ; i++) {
			 * System.out.println("IN START" + (int)rxbuffer[i]);
			 * //System.out.println("IN START" + (int)rxbuffer[i+1]); if(i <
			 * (rxbuffer.length -1 ) && rxbuffer[i]== (byte)0xFF && rxbuffer[i+1]==
			 * (byte)0xD8) {
			 * 
			 * System.out.println("\n\n\nSTART\n\n\n\n" + rxbuffer[i] + "   " +
			 * rxbuffer[i+1]); image.write(rxbuffer[i]); image.write(rxbuffer[i+1]); start =
			 * i+2; startflag=true; for(int j = start; j < rxbuffer.length; j++)
			 * image.write(rxbuffer[j]); //break;
			 * 
			 * }
			 * 
			 * }
			 * 
			 * }catch(IOException e) {;} }
			 * 
			 */

			sendPacket(imgCode, serverPort);
			try {
				r.receive(q);
				for (int i = 0; i < rxbuffer.length; i++) {
					System.out.println(rxbuffer[i] + " " + i);
					if (i < (rxbuffer.length - 1) && rxbuffer[i] == (byte) 0xFF && rxbuffer[i + 1] == (byte) 0xD9) {
						System.out.println("END");
						image.write(rxbuffer[i]);
						image.write(rxbuffer[i + 1]);
						endflag = true;
						break;
					}

					image.write(rxbuffer[i]);

				}

			} catch (IOException e) {
				;
			}

		}
		r.close();

	}

	public void getAudio(byte[] audioCode, int audioPackets, int samplerate, String encoding, int filenum,
			boolean genmode) { // genmode == true if i have gennhtria

		int mean = 0;
		int b = 1;
		int header = 0;
		int Q = 8;
		int buffersize = 1; // Depending on type of encoding (basically a function of Q) if Q = 8 then
							// buffersize =1 else if Q = 16 buffersize = 2
							// Will be used to allocate appropriate space for audioBuffer depending on DPCM
							// or AQ-DPCM encoding.

		if (encoding == "AQ-DPCM") {
			header = 4;
			Q = 16;
			buffersize = 2;
		}

		DatagramSocket r = null;

		try {
			r = new DatagramSocket(clientPort);
			r.setSoTimeout(3500); // 3500 to get no timeouts before packet
		} catch (SocketException e) {
			System.out.println(e);
		}

		byte[] rxbuffer = new byte[128 + header];
		DatagramPacket q = new DatagramPacket(rxbuffer, rxbuffer.length);

		int bLSB; // LSB byte of beta
		int bMSB; // MSM byte of beta
		int bAQ; // value of beta in AQ-DPCM

		int mLSB; // LSB byte of m average
		int mMSB; // MSM byte of m average
		int mAQ;

		int packetNum = audioPackets;
		int[] nibblesamples = new int[256];
		int nibble1;
		int nibble2;

		int diff1;
		int diff2;
		int sample0 = 0;
		int sample1 = 0;
		int sample2 = 0;

		byte[] audioBufferS = new byte[buffersize * packetNum * 256];
		int[] demux = new int[256];
		SourceDataLine lineOut = null;

		try {

			AudioFormat linearPCM = new AudioFormat(samplerate, Q, 1, true, false);
			lineOut = AudioSystem.getSourceDataLine(linearPCM);
			lineOut.open(linearPCM, 256 * packetNum);
			lineOut.start();
		} catch (LineUnavailableException e) {
			System.out.println(e);
		}

		PrintWriter meanfile = null, stepfile = null, samples = null, dpcmfreq = null, diffsDPCM = null, diffsAQ = null;

		try {
			if (encoding == "DPCM") {
				if (!genmode) {
					diffsDPCM = new PrintWriter("samples/session2/diffsDPCM.txt", "UTF-8");
					dpcmfreq = new PrintWriter("samples/session2/dpcmfreq.txt", "UTF-8");
				} else {
					diffsDPCM = new PrintWriter("samples/session2/diffsDPCMGEN.txt", "UTF-8");
					dpcmfreq = new PrintWriter("samples/session2/dpcmfreqGEN.txt", "UTF-8");
				}
			}
			if (encoding == "AQ-DPCM") {
				meanfile = new PrintWriter("samples/session2/meanfile" + filenum + ".txt", "UTF-8");
				diffsAQ = new PrintWriter("samples/session2/diffsAQ" + filenum + ".txt", "UTF-8");
				stepfile = new PrintWriter("samples/session2/stepfile" + filenum + ".txt", "UTF-8");
				samples = new PrintWriter("samples/session2/samples" + filenum + ".txt", "UTF-8");
			}
		} catch (Exception e) {
			;
		}

		sendPacket(audioCode, serverPort);

		for (int j = 0; j < packetNum; j++) {

			try {
				r.receive(q);

				bLSB = (int) (rxbuffer[2] & 0xFF); // chooses the LSB byte of beta from header, &0xFF: to make variable
													// unsigned
				bMSB = (int) (rxbuffer[3] & 0xFF); // chooses the MSB byte of beta from header
				bAQ = bMSB * 256 + bLSB; // 256 because it is 2^8=256

				mLSB = (int) (rxbuffer[0] & 0xFF);
				mMSB = (int) (rxbuffer[1]);
				mAQ = mMSB * 256 + mLSB;

				if (encoding == "AQ-DPCM") {
					mean = mAQ;
					b = bAQ;

				}

				if ((packetNum - j) % 100 == 0) {
					System.out.print((packetNum - j) / 10 + "% Remaining\r");
				}

				for (int i = 0 + header; i < rxbuffer.length; i++) {

					nibble1 = (byte) ((rxbuffer[i] & 240) >> 4); // 240 -> 11110000 >> 4 cause little-endian
					nibble2 = (byte) (rxbuffer[i] & 15); // 15 -> 00001111

					nibblesamples[i - header] = (int) nibble1;
					nibblesamples[i + 1 - header] = (int) nibble2;

					diff1 = (nibblesamples[i - header] - 8) * b;
					diff2 = (nibblesamples[i + 1 - header] - 8) * b;

					/*
					 * sample1=diff1 +m; sample2=diff1+diff2+m; sample0=sample2;
					 */

					if (encoding == "DPCM") {
						sample1 = diff1 + sample0;
						sample2 = diff2 + sample1;
						sample0 = sample2; /// Correct DPCM
						dpcmfreq.println(sample1);
						dpcmfreq.println(sample2);
						diffsDPCM.println((int) nibble1 - 8);
						diffsDPCM.println((int) nibble2 - 8);
					}

					if (encoding == "AQ-DPCM") {
						sample1 = sample0 + diff1 + mean;
						sample2 = diff1 + diff2 + mean; /// Το καλυτερο για AQ
						sample0 = diff2;

						samples.println(sample1);
						samples.println(sample2);
						diffsAQ.println((int) nibble1 - 8);
						diffsAQ.println((int) nibble2 - 8);

					}

					demux[(i - header) * 2] = sample1;
					demux[(i - header) * 2 + 1] = sample2;

				}

				if (encoding == "DPCM") {

					for (int i = 0; i < rxbuffer.length; i++) {
						audioBufferS[256 * j + i * 2] = (byte) demux[i * 2];
						audioBufferS[256 * j + i * 2 + 1] = (byte) demux[i * 2 + 1];

					}

				}
				if (encoding == "AQ-DPCM") {
					for (int i = 0; i < rxbuffer.length - 4; i++) {
						audioBufferS[512 * j + i * 4] = (byte) (demux[i * 2] & 0xFF);
						audioBufferS[512 * j + i * 4 + 1] = (byte) ((demux[i * 2] >> 8) & 0xFF);
						audioBufferS[512 * j + i * 4 + 2] = (byte) (demux[i * 2 + 1] & 0xFF);
						audioBufferS[512 * j + i * 4 + 3] = (byte) ((demux[i * 2 + 1] >> 8) & 0xFF);

					}
					System.out.println(b);
					meanfile.println(mean);
					stepfile.println(b);

				}

			} catch (IOException e) {
				;
			}
		}

		if (encoding == "AQ-DPCM") {
			meanfile.close();
			stepfile.close();
			samples.close();
			diffsAQ.close();

		}
		if (encoding == "DPCM") {
			dpcmfreq.close();
			diffsDPCM.close();

		}

		lineOut.write(audioBufferS, 0, buffersize * 256 * packetNum);

		lineOut.stop();

		lineOut.close();

		r.close();

	}

	public void ithakiCopter(byte[] copterCode) {

		DatagramSocket r = null;
		byte[] rxbuffer = new byte[113];
		DatagramPacket q = new DatagramPacket(rxbuffer, rxbuffer.length);

		long startTime = System.currentTimeMillis();
		int interval = 60000; // 1 λεπτό λειτουργίασ
		long endTime = startTime + interval;

		String leftmotor, rightmotor, altitude, temperature, pressure;
		PrintWriter lmotor = null, rmotor = null, alt = null, temp = null, press = null;
		try {

			lmotor = new PrintWriter("samples/session2/lmotor.txt");
			rmotor = new PrintWriter("samples/session2/rmotor.txt");
			alt = new PrintWriter("samples/session2/altitude.txt");
			temp = new PrintWriter("samples/session2/temperaturecopter.txt");
			press = new PrintWriter("samples/session2/pressurecopter.txt");
			r = new DatagramSocket(48038);
			r.setSoTimeout(3500); // 3500 to get no timeouts before packet

		} catch (IOException e) {
			System.out.println(e);
		}

		while (System.currentTimeMillis() < endTime) {

			try {
				r.receive(q);
				String message = new String(rxbuffer, 0, q.getLength());
				System.out.println(message);
				leftmotor = message.substring(40, 43);
				System.out.println("LMOTOR = " + leftmotor);
				rightmotor = message.substring(51, 54);
				System.out.println("RMOTOR = " + rightmotor);
				altitude = message.substring(64, 67);
				System.out.println("ALTITUDE = " + altitude);
				temperature = message.substring(80, 86);
				System.out.println("TEMPERATURE = " + temperature);
				pressure = message.substring(96, 103);
				System.out.println("PRESSURE = " + pressure);
				lmotor.println(leftmotor);
				rmotor.println(rightmotor);
				alt.println(altitude);
				temp.println(temperature);
				press.println(pressure);

			} catch (IOException e) {
				System.out.println(e);
			}

		}
		lmotor.close();
		rmotor.close();
		alt.close();
		temp.close();
		press.close();

		r.close();

	}

	public void vehicleOBDiagnostics(byte[] OBDcode, String pid, String filename) {

		long startTime = System.currentTimeMillis();
		int interval = 4 * 60000; // 5 λεπτά λειτουργίας
		long endTime = startTime + interval;

		byte[] rxbuffer = new byte[113];
		DatagramPacket q = new DatagramPacket(rxbuffer, rxbuffer.length);

		DatagramSocket r = null;
		PrintWriter file = null;
		try {
			file = new PrintWriter(filename, "UTF-8");
			r = new DatagramSocket(clientPort);
			r.setSoTimeout(3500); // 3500 to get no timeouts before packet

		} catch (IOException e) {
			System.out.println(e);
		}

		int formula = 0;
		String units;
		int xx, yy;
		System.out.println("OUTPUTTING CODE : " + pid);
		String message = "";
		while (System.currentTimeMillis() < endTime) {

			try {

				sendPacket(OBDcode, serverPort);
				r.receive(q);
				message = new String(rxbuffer, 0, q.getLength());
				System.out.println(message);
				// System.out.println(message.substring(6,8)); // XX

			} catch (IOException e) {
				System.out.println(e);

			}

			xx = Integer.parseInt(message.substring(6, 8), 16);

			switch (pid) {

			case "1F":

				yy = Integer.parseInt(message.substring(9, 11), 16);
				formula = 256 * xx + yy;
				System.out.println("Engine runtime : " + formula + " sec");
				break;
			case "0F":
				// xx = Integer.parseInt(message.substring(6,8), 16 );
				formula = xx - 40;
				System.out.println("Intake air temperature: " + formula + " (deg C)");
				break;
			case "11":
				formula = xx * 100 / 255;
				System.out.println("Throttle position:  " + formula + " %");
				break;
			case "0C":
				yy = Integer.parseInt(message.substring(9, 11), 16);
				formula = ((xx * 256) + yy) / 4;
				System.out.println("Engine RPM:  " + formula + " rpm");
				break;
			case "0D":
				formula = xx;
				System.out.println("Vehicle speed:   " + formula + " km/h");
				break;
			case "05":
				formula = xx - 40;
				System.out.println("Coolant temperature: " + formula + " (deg) C");
				break;

			}
			file.println(formula);
		}
		r.close();
		file.close();
	}

}
