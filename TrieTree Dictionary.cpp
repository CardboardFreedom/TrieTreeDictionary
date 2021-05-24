
//

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>

class Tree
{
	Tree* something[26] = { nullptr };

	Tree(char i) { w = i; }

	bool word = false;
	char w = '/';

public:

	Tree(){}

	~Tree()
	{
		for (int i = 25; i >= 0; i--)
			delete something[i];
	}

	char wValue() { return w; }

	void insertLetter(char x) { w = x; }

	void insert(std::string wo)
	{
		Tree* look = this;

		for (int i = 1; i < wo.size(); i++)
		{
			if (look->something[wo[i] - 'A'] == nullptr)
				look->something[wo[i] - 'A'] = new Tree(wo[i]);

			look = look->something[wo[i] - 'A'];
		}

		if (look != nullptr)
			look->word = true;
	}

	Tree* find(std::string f)
	{
		f.erase(0, 1);

		if (f.size() == 0)
			return (word ? nullptr : this);

		if (something[f[0] - 'A'] != nullptr)
			return something[f[0] - 'A']->find(f);

		return this;
	}

	void suggest(std::string org, std::string& out)
	{
		for (int i = 0; i < 26; i++)
		{
			if (this->something[i] != nullptr)
			{
				if(this->something[i]->word == true)
					out += (org + this->something[i]->w + ", ");
				this->something[i]->suggest(org + this->something[i]->w, out);
			}
		}
	}
};

int main()
{
	auto t0 = std::chrono::steady_clock::now();
	std::ifstream inFile;
	inFile.open("dictionary.txt");

	if (!inFile.is_open())
	{
		std::cout << "ERROR: Couldn't open File\n";
		return 0;
	}

	Tree d[26];

	for (int i = 0; i < 26; i++)
		d[i].insertLetter('A' + i);

	std::string words;

	while (inFile >> words)
	{
		transform(words.begin(), words.end(), words.begin(), ::toupper);
		d[words[0] - 'A'].insert(words);
	}

	inFile.close();

	auto t1 = std::chrono::steady_clock::now();
	auto dd = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);
	std::cout << dd.count() << " seconds" << std::endl;

	words.clear();

	while (true)
	{
		std::cout << "Enter Word: ";
		std::cin >> words;

		transform(words.begin(), words.end(), words.begin(), ::toupper);

		if (words == "/")
		{
			std::cout << "Closing Program. Goodbye!\n";
			return 0;
		}

		t0 = std::chrono::steady_clock::now();

		if (!isalpha(words[0]))
		{
			std::cout << "Invalid Character Found. Only Alphabetical Values Can Be Used.\n";
			continue;
		}
		auto ptr = d[(words[0] - 'A')].find(words);

		if (ptr == nullptr) std::cout << "Word FOUND!\n\n";

		else if (ptr->wValue() != words[words.size()-1])
			std::cout << "Word Not Found - No Suggestions!\n\n";

		else
		{
			std::cout << "Word Not Found!\n\n";
			std::string suggestions;
			ptr->suggest(words, suggestions);

			if (suggestions.size() != 0)
			{
				std::cout << "Suggestions:\n";
				suggestions.erase(suggestions.size() - 2, 2);
				std::cout << suggestions << "\n\n";
			}
		}

		t1 = std::chrono::steady_clock::now();
		dd = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);
		std::cout << dd.count() << " seconds" << std::endl;
	}
}