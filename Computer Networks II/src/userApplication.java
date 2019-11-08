package userApplication;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import javax.sound.sampled.*;
import java.util.*;
import java.io.*;

public class userApplication {

	int clientPort = 48012;
	int serverPort = 38012;

	public static void main(String[] args)
			throws SocketException, IOException, UnknownHostException, LineUnavailableException {

		userApplication self = new userApplication();

		byte[] echoCode = "E1445".getBytes();
		// self.echoPackets(echoCode,"samples/session1/responseEcho.txt");
		byte[] imgCode = ("M4181FLOW=ON" + "UDP=1024").getBytes();
		// self.cameraImage(imgCode, "samples/session1/E1.jpg");
		int audioPackets = 900;
		byte[] audioCodeAQ = ("A0604AQL55F" + audioPackets).getBytes();
		byte[] audioCode = ("A0604L55F" + audioPackets).getBytes();
		self.getAudio(audioCode, audioPackets, 8000, "DPCM");
		// self.getAudio(audioCodeAQ,audioPackets, 8000, "AQ-DPCM");

		// self.audioAQ(audioCodeAQ, audioPackets, serverPort);
		// self.soundAQDPCM(5509, 1, 38025, 48025);
		System.out.println();
	}

	public void sendPacket(byte[] txbuffer) {

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
		packet.setPort(serverPort);

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

		byte[] rxbuffer = new byte[32]; // 32 bytes echo packet
		DatagramPacket q = new DatagramPacket(rxbuffer, rxbuffer.length);
		String message = null;

		long startTime = System.currentTimeMillis();
		int interval = 60000; // 5 λεπτά λειτουργίασ
		long endTime = startTime + interval;

		while (System.currentTimeMillis() < endTime) {

			sendPacket(echoCode);

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

			sendPacket(imgCode);
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

	public void getAudio(byte[] audioCode, int audioPackets, int samplerate, String encoding) {

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

		int blsb; // LSB byte of beta
		int bmsb; // MSM byte of beta
		int baq; // value of beta in AQ-DPCM

		int mlsb; // LSB byte of m average
		int mmsb; // MSM byte of m average
		int maq;

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
		String progress = "";
		SourceDataLine lineOut = null;

		try {

			AudioFormat linearPCM = new AudioFormat(samplerate, Q, 1, true, false);
			lineOut = AudioSystem.getSourceDataLine(linearPCM);
			lineOut.open(linearPCM, 256 * packetNum);
			lineOut.start();
		} catch (LineUnavailableException e) {
			System.out.println(e);
		}

		sendPacket(audioCode);

		for (int j = 0; j < packetNum; j++) {

			try {
				r.receive(q);

				blsb = (int) (rxbuffer[2] & 0xFF); // chooses the LSB byte of beta from header, &0xFF: to make variable
													// unsigned
				bmsb = (int) (rxbuffer[3] & 0xFF); // chooses the MSB byte of beta from header
				baq = bmsb * 256 + blsb; // 256 because it is 2^8=256

				mlsb = (int) (rxbuffer[0] & 0xFF);
				mmsb = (int) (rxbuffer[1]);
				maq = mmsb * 256 + mlsb;

				if (encoding == "AQ-DPCM") {
					mean = maq;
					b = baq;

				}

				if ((packetNum - j) % 100 == 0) {
					System.out.print((packetNum - j) / 10 + "% Remaining\r");
				}

				for (int i = 0 + header; i < rxbuffer.length; i++) {
					// System.out.print(rxbuffer[i]);
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
					}

					if (encoding == "AQ-DPCM") {
						sample1 = sample0 + diff1 + mean;
						sample2 = diff1 + diff2 + mean; /// Το καλυτερο για AQ
						sample0 = diff2;
					}

					// audioBufferS[j*256 + (i-header)*2]=(byte)sample1;
					// audioBufferS[j*256 + (i-header)*2+1]=(byte)sample2;
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

				}

				// System.out.println();
			} catch (IOException e) {
				;
			}
		}

		lineOut.write(audioBufferS, 0, buffersize * 256 * packetNum);

		lineOut.stop();

		lineOut.close();

		r.close();

	}

}
