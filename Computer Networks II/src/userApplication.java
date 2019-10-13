
package userApplication;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.io.IOException;


public class userApplication {

	
	static int clientPort = 48031;
	static int serverPort = 38031;
	public static void main(String[] args) {
		String echo = "E1138";
		echoPackets(echo,20);

	}
	
	public static void sendPacket(String packetInf) {
		
		DatagramSocket sock = null;
		
		try {
			sock = new DatagramSocket();
		}
		catch(SocketException e) {
			System.out.println(e);
		}
		
		
		byte[] serverIP = {(byte)155, (byte)207, (byte)18, (byte)208};  // Ithaki IP
		byte[] txbuffer = packetInf.getBytes();
		
		InetAddress serverAddress = null;
		
		try{
			serverAddress = InetAddress.getByAddress(serverIP);
		}
		catch(UnknownHostException e) {
			System.out.println(e);
		}
		
		DatagramPacket packet = new DatagramPacket(txbuffer,txbuffer.length);
		packet.setAddress(serverAddress);
		packet.setPort(serverPort);
		
		try{
			sock.send(packet);
		}
		catch(IOException e) {
			System.out.println(e);
		}
		
		
		
		
	}

	public static void echoPackets(String echoCode,int packetCount) {
		DatagramSocket r = null;
		
		
		try {
			r = new DatagramSocket(clientPort);
			r.setSoTimeout(2200); // 2200 to get no timeouts before packet
		}
		catch(SocketException e) {
			System.out.println(e);
		}
		
		
		byte[] rxbuffer = new byte[32]; // 32 bytes echo packet
		DatagramPacket q = new DatagramPacket(rxbuffer,rxbuffer.length);
		String message = null;
		
		for(int i = 0 ; i < packetCount ; i++) {
			
			sendPacket(echoCode);
			try {
				r.receive(q);
				message = new String(rxbuffer,0,q.getLength());
				System.out.println(message);
			}catch(IOException e) {
				System.out.println(e);
			}
			
		}
		
	}

}
