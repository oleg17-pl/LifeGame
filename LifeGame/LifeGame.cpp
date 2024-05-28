#include <iostream>
#include <random>
#include <conio.h>

void clearMemory(const int size, bool** const field)
{
	for (int i = 0; i < size; i++)
	{
		delete[] field[i];
	}
	delete[] field;
}

bool** createField(const int sizex, const int sizey)
{
	bool** field = new bool* [sizey];
	for (int i = 0; i < sizey; i++)
	{
		field[i] = new bool[sizex];
	}
	return field;
}

class Game
{
private:
	bool** field;
	int fieldSizeX;
	int fieldSizeY;

	bool checkIsAlive(const int x, const int y) const
	{
		if (x < 0 || y < 0 || x >= fieldSizeX || y >= fieldSizeY)
			return false;
		return field[y][x];
	}

public:
	Game(int fieldSizeX, int fieldSizeY, bool** field) :
		fieldSizeX{ fieldSizeX },
		fieldSizeY{ fieldSizeY },
		field{ field }
	{

	}

	void ProcessMove()
	{
		bool** newField{ createField(fieldSizeX, fieldSizeY) };

		for (int i = 0; i < fieldSizeY; i++)
		{
			for (int j = 0; j < fieldSizeX; j++)
			{
				int neighbours{};

				for (int y = -1; y <= 1; y++)
				{
					for (int x = -1; x <= 1; x++)
					{
						if (x != 0 || y != 0)
						{
							neighbours += checkIsAlive(j + x, i + y);
						}
					}
				}

				if (neighbours < 2 || neighbours > 3)
				{
					newField[i][j] = 0;
				}
				else if (neighbours == 3)
				{
					newField[i][j] = 1;
				}
				else
				{
					newField[i][j] = field[i][j];
				}
			}
		}

		clearMemory(fieldSizeY, field);
		field = newField;
	}

	void PrintField() const
	{
		std::cout << std::endl;
		for (int i = 0; i < fieldSizeY; i++)
		{
			for (int j = 0; j < fieldSizeX; j++)
			{
				char cell = field[i][j] == 1 ? '#' : '.';
				std::cout << cell;
			}
			std::cout << std::endl;
		}
	}

	int countAliveCells() const
	{
		int count{};
		for (int i = 0; i < fieldSizeY; i++)
		{
			for (int j = 0; j < fieldSizeX; j++)
			{
				count += checkIsAlive(j, i);
			}
		}
		return count;
	}

	~Game()
	{
		clearMemory(fieldSizeY, field);
	}
};

bool** initField(const int sizex, const int sizey)
{
	bool** field = createField(sizex, sizey);
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<> distribution(0, 1);

	for (int i = 0; i < sizey; i++)
	{
		for (int j = 0; j < sizex; j++)
		{
			field[i][j] = distribution(generator);
		}
	}
	return field;
}

int main()
{
	int sizex{}, sizey{};
	std::cout << "Enter x size: ";
	std::cin >> sizex;
	std::cout << "Enter y size: ";
	std::cin >> sizey;

	bool** field{ initField(sizex, sizey) };
	Game game(sizex, sizey, field);
	bool isGame = true;

	system("cls");

	while (isGame) 
	{
		std::cout << "Life Game" << std::endl;
		game.PrintField();
		std::cout << std::endl << "Alive cells: " << game.countAliveCells();
		std::cout << std::endl << "Press zero to exit...";

		char input{ (char)_getch() };
		isGame = input != '0';

		game.ProcessMove();
		system("cls");
	}

	return 0;
}