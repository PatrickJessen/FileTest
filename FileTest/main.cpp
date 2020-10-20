#include <fstream>
#include <iostream>
#include <vector>
#include <thread>

short mapArray[8][8] =
{
	600,600,600,600,600,600,600,600,
	600,0, 0, 0, 0, 0, 0, 600,
	600,0, 0, 0, 0, 0, 0, 600,
	600,0, 0, 0, 0, 0, 0, 600,
	600,0, 0, 0, 0, 0, 0, 600,
	600,0, 0, 0, 0, 0, 0, 600,
	600,0, 0, 0, 0, 0, 0, 600,
	600,600,600,600,600,600,600,600,
};

short loaded[8][8];

class ReadFile
{
private:
	const char* filename;
	std::basic_ifstream<unsigned char> inFile;

public:
	ReadFile(const char* filename)
	{
		this->filename = filename;
		inFile = std::basic_ifstream<uint8_t>(filename, std::ifstream::binary);

		if (inFile.fail())
		{
			std::cout << "Couln't load file!";
		}
	}
	char ReadChar()
	{
		uint8_t byte = 0;
		inFile.read(&byte, 1);
		return byte;
	}
	short ReadShort()
	{
		uint8_t bytes[2];
		inFile.read(bytes, 2);
		return (bytes[1] << 8) | (bytes[0]);
	}
	int ReadInt()
	{
		uint8_t bytes[4];
		inFile.read(bytes, 4);
		return (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | (bytes[0]);
	}

	void* ReadType(int size)
	{
		unsigned char* any = new unsigned char[size];
		inFile.read(any, size);
		return (void*)any;
	}
	void ReadSkip(uint32_t amount)
	{
		for (uint32_t i = 0; i < amount; i++)
			ReadChar();
	}
	void Close()
	{
		inFile.close();
	}
};
class WriteFile
{
private:
	const char* filename;
	std::basic_fstream<unsigned char> outFile;

public:
	WriteFile(const char* filename)
	{
		this->filename = filename;
		outFile = std::basic_fstream<uint8_t>(filename, std::ios::out | std::ios::binary);
		if (!outFile.is_open())
		{
			std::cout << "Couln't open file!\n";
		}
	}
	void WriteChar(char byte)
	{
		outFile.write((unsigned char*)&byte, 1);
	}
	void WriteShort(short word)
	{
		unsigned char bytes[2];
		bytes[0] = word & 0x000000ff;
		bytes[1] = (word & 0x0000ff00) >> 8;

		outFile.write(bytes, 2);
	}
	void WriteInt(int dword)
	{
		unsigned char bytes[4];
		bytes[0] = dword & 0x000000ff;
		bytes[1] = (dword & 0x0000ff00) >> 8;
		bytes[2] = (dword & 0x00ff0000) >> 16;
		bytes[3] = (dword & 0xff000000) >> 24;

		outFile.write(bytes, 4);
	}
	void WriteType(void* any, int size)
	{
		outFile.write((unsigned char*)any, size);
	}

	void Close()
	{
		outFile.close();
	}
};


class Player
{
public:
	int x;
	int y;
	int angle;

	Player(int x, int y, int angle)
		: x(x), y(y), angle(angle)
	{

	}
};

int main()
{
	std::vector<Player*> players;

	for (int i = 0; i < 8; i++)
	{
		players.push_back(new Player(rand() % 10, rand() % 1000, rand() % 100000));
		std::cout << players[i]->x << ' ' << players[i]->y << ' ' << players[i]->angle << ' ';
	}
	std::cout << '\n';
	WriteFile writeFile("map.lvl");

	writeFile.WriteInt(players.size());

	for (int i = 0; i < players.size(); i++)
		writeFile.WriteType(players[i], sizeof(Player));

	writeFile.Close();

	std::vector<Player*> playerLoad;
	
	ReadFile readFile("map.lvl");
	int playerLength = readFile.ReadInt();
	for (int i = 0; i < playerLength; i++)
	{
		playerLoad.push_back((Player*)readFile.ReadType(sizeof(Player)));
		std::cout << playerLoad[i]->x << ' ' << playerLoad[i]->y << ' ' << playerLoad[i]->angle << ' ';
	}
	//Player* loadedPlayer = (Player*)readFile.ReadType(sizeof(Player));
	//std::cout << loadedPlayer->x << ' ' << loadedPlayer->y << ' ' << loadedPlayer->angle;
	readFile.Close();
	


	//std::ofstream outFile("map.lvl", std::ios::binary);

	//outFile.close();
}