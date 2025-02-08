/*
	Reliability and Flow Control Example
	From "Networking for Game Programmers" - http://www.gaffer.org/networking-for-game-programmers
	Author: Glenn Fiedler <gaffer@gaffer.org>
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Net.h"
#include "file_Transfer.h"
#include "performance.h"
#include "protocol.h"

//#define SHOW_ACKS

using namespace std;
using namespace net;

const int ServerPort = 30000;
const int ClientPort = 30001;
//const int ProtocolId = 0x11223344;
const float DeltaTime = 1.0f / 30.0f;
const float SendRate = 1.0f / 30.0f;
const float TimeOut = 10.0f;
const int PacketSize = 256;

class FlowControl
{
public:

	FlowControl()
	{
		printf("flow control initialized\n");
		Reset();
	}

	void Reset()
	{
		mode = Bad;
		penalty_time = 4.0f;
		good_conditions_time = 0.0f;
		penalty_reduction_accumulator = 0.0f;
	}

	void Update(float deltaTime, float rtt)
	{
		const float RTT_Threshold = 250.0f;

		if (mode == Good)
		{
			if (rtt > RTT_Threshold)
			{
				printf("*** dropping to bad mode ***\n");
				mode = Bad;
				if (good_conditions_time < 10.0f && penalty_time < 60.0f)
				{
					penalty_time *= 2.0f;
					if (penalty_time > 60.0f)
						penalty_time = 60.0f;
					printf("penalty time increased to %.1f\n", penalty_time);
				}
				good_conditions_time = 0.0f;
				penalty_reduction_accumulator = 0.0f;
				return;
			}

			good_conditions_time += deltaTime;
			penalty_reduction_accumulator += deltaTime;

			if (penalty_reduction_accumulator > 10.0f && penalty_time > 1.0f)
			{
				penalty_time /= 2.0f;
				if (penalty_time < 1.0f)
					penalty_time = 1.0f;
				printf("penalty time reduced to %.1f\n", penalty_time);
				penalty_reduction_accumulator = 0.0f;
			}
		}

		if (mode == Bad)
		{
			if (rtt <= RTT_Threshold)
				good_conditions_time += deltaTime;
			else
				good_conditions_time = 0.0f;

			if (good_conditions_time > penalty_time)
			{
				printf("*** upgrading to good mode ***\n");
				good_conditions_time = 0.0f;
				penalty_reduction_accumulator = 0.0f;
				mode = Good;
				return;
			}
		}
	}

	float GetSendRate()
	{
		return mode == Good ? 30.0f : 10.0f;
	}

private:

	enum Mode
	{
		Good,
		Bad
	};

	Mode mode;
	float penalty_time;
	float good_conditions_time;
	float penalty_reduction_accumulator;
};

// flow chart defination remains unchanged 

int main(int argc, char* argv[])
{
	// parse command line

	
	enum Mode
	{
		Client,
		Server
	};

	Mode mode = Server;
	Address address;

	/* adding the additional commandline argument here agrv >=3
	* 
	* - for the client file path can send via this arguments for sending metadata 
	* - and for the server the loaction of file to saved for receiving metadat 
	* - having a default file name for receving the data 
	* - mode setup in this remain unchanges as it is in argv 2 
	*
	*/
	if (argc >= 2)
	{
		int a, b, c, d;
		if (sscanf_s(argv[1], "%d.%d.%d.%d", &a, &b, &c, &d))
		{
			mode = Client;
			address = Address(a, b, c, d, ServerPort);
		}
	}

	// initialize

	if (!InitializeSockets())
	{
		printf("failed to initialize sockets\n");
		return 1;
	}

	ReliableConnection connection(ProtocolId, TimeOut);

	const int port = mode == Server ? ServerPort : ClientPort;

	if (!connection.Start(port))
	{
		printf("could not start connection on port %d\n", port);
		return 1;
	}

	if (mode == Client)
		connection.Connect(address);
	else
		connection.Listen();

	bool connected = false;
	float sendAccumulator = 0.0f;
	float statsAccumulator = 0.0f;

	FlowControl flowControl;

	while (true)
	{
		// update flow control

		if (connection.IsConnected())
			flowControl.Update(DeltaTime, connection.GetReliabilitySystem().GetRoundTripTime() * 1000.0f);

		const float sendRate = flowControl.GetSendRate();

		// detect changes in connection state

		if (mode == Server && connected && !connection.IsConnected())
		{
			flowControl.Reset();
			printf("reset flow control\n");
			connected = false;
		}

		if (!connected && connection.IsConnected())
		{
			printf("client connected to server\n");
			connected = true;
		}

		if (!connected && connection.ConnectFailed())
		{
			printf("connection failed\n");
			break;
		}

		// send and receive packets

		sendAccumulator += DeltaTime;

		while (sendAccumulator > 1.0f / sendRate)
		{
			unsigned char packet[PacketSize];
			memset(packet, 0, sizeof(packet));
			connection.SendPacket(packet, sizeof(packet));
			sendAccumulator -= 1.0f / sendRate;
		}


			/*
    {
        //  
        // 2. Client tasks
        //  
        if (mode == Client && connection.IsConnected())
        {
            // a) Retrieving the file from disk
            // Open the specified file, read its contents, and store in memory
            // Use std::ifstream to read the file in binary mode
            // Example:
            // std::ifstream file(filePath, std::ios::binary);

            // b) Sending file metadata
            // Send information such as file name and size to the server
            // Create a metadata structure and serialize it into a packet
            // Example structure:
            // struct FileMetadata { char fileName[256]; unsigned int fileSize; };

            // c) Breaking the file into pieces to send
            // Split the file into chunks of `PacketSize` and prepare to send each chunk

            // d) Sending the pieces
            // Use `connection.SendPacket()` to send each chunk to the server
            // Example:
            // connection.SendPacket(dataChunk, chunkSize);
        }
		 
        // 3. Server tasks
        //  
        if (mode == Server && connection.IsConnected())
        {
            // a) Receiving the file metadata
            // Receive and parse the metadata to determine the file size and name
            // Example:
            // FileMetadata metadata;
            // connection.ReceivePacket((unsigned char*)&metadata, sizeof(metadata));

            // b) Receiving the file pieces
            // Accumulate received file chunks into a buffer

            // c) Writing the pieces out to disk
            // Save the reassembled file to the specified file path or default location
            // Use std::ofstream in binary mode

            // d) Verifying the file integrity
            // Compare the received file's size or hash with the metadata to ensure no corruption
        }

        // Existing flow control and packet processing remains unchanged
			*/

			
		if (mode == Server && connection.IsConnected()) {
			Packet receivedPacket;
			int bytesRead = connection.ReceivePacket(reinterpret_cast<unsigned char*>(&receivedPacket), sizeof(receivedPacket));

			if (bytesRead > 0 && receivedPacket.packetId == ProtocolId) {
				double startTime = Performance::getCurrentTime();  // Start time when file starts arriving

				std::ofstream outFile("received_file", std::ios::binary);
				outFile.write(receivedPacket.data, receivedPacket.dataSize);
				outFile.close();

				double endTime = Performance::getCurrentTime();  // End time after file has been received

				unsigned int crcReceived = FileTransfer::calculateCRC(receivedPacket.data, receivedPacket.dataSize);
				printf("File Received: %d bytes, CRC: %u\n", receivedPacket.dataSize, crcReceived);

				// Measure transfer speed
				Performance::measureTransferSpeed(receivedPacket.dataSize, startTime, endTime);
			}
		}


		if (mode == Server && connection.IsConnected()) {
			Packet receivedPacket;
			int bytesRead = connection.ReceivePacket(reinterpret_cast<unsigned char*>(&receivedPacket), sizeof(receivedPacket));

			if (bytesRead > 0 && receivedPacket.packetId == ProtocolId) {
				double startTime = Performance::getCurrentTime();  // Start time when file starts arriving

				std::ofstream outFile("received_file", std::ios::binary);
				outFile.write(receivedPacket.data, receivedPacket.dataSize);
				outFile.close();

				double endTime = Performance::getCurrentTime();  // End time after file has been received

				unsigned int crcReceived = FileTransfer::calculateCRC(receivedPacket.data, receivedPacket.dataSize);
				printf("File Received: %d bytes, CRC: %u\n", receivedPacket.dataSize, crcReceived);

				// Measure transfer speed
				Performance::measureTransferSpeed(receivedPacket.dataSize, startTime, endTime);
			}
		}


			


		/* {
			unsigned char packet[256];
			int bytes_read = connection.ReceivePacket(packet, sizeof(packet));
			if (bytes_read == 0)
				break;
		}*/

		// show packets that were acked this frame

#ifdef SHOW_ACKS
		unsigned int* acks = NULL;
		int ack_count = 0;
		connection.GetReliabilitySystem().GetAcks(&acks, ack_count);
		if (ack_count > 0)
		{
			printf("acks: %d", acks[0]);
			for (int i = 1; i < ack_count; ++i)
				printf(",%d", acks[i]);
			printf("\n");
		}
#endif

		// update connection

		connection.Update(DeltaTime);

		// show connection stats

		statsAccumulator += DeltaTime;

		while (statsAccumulator >= 0.25f && connection.IsConnected())
		{
			float rtt = connection.GetReliabilitySystem().GetRoundTripTime();

			unsigned int sent_packets = connection.GetReliabilitySystem().GetSentPackets();
			unsigned int acked_packets = connection.GetReliabilitySystem().GetAckedPackets();
			unsigned int lost_packets = connection.GetReliabilitySystem().GetLostPackets();

			float sent_bandwidth = connection.GetReliabilitySystem().GetSentBandwidth();
			float acked_bandwidth = connection.GetReliabilitySystem().GetAckedBandwidth();

			printf("rtt %.1fms, sent %d, acked %d, lost %d (%.1f%%), sent bandwidth = %.1fkbps, acked bandwidth = %.1fkbps\n",
				rtt * 1000.0f, sent_packets, acked_packets, lost_packets,
				sent_packets > 0.0f ? (float)lost_packets / (float)sent_packets * 100.0f : 0.0f,
				sent_bandwidth, acked_bandwidth);

			statsAccumulator -= 0.25f;
		}

		net::wait(DeltaTime);
	}

	ShutdownSockets();

	return 0;
}
