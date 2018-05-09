#include <iostream>
#include <vector>
#include <iomanip>
#include <set>
#include <time.h>

const int MAX = 100000;

struct Vertex{
	int weight;
	int pastVertexIndex;
	void out(){
		std::cout << "вес= " << weight << " ,индекс пред. вер. = " << pastVertexIndex + 1<< "\n";
	}
};

void dynprFunc(int** TableOfRoutes, int Size){
	std::vector<Vertex> VectorOfVertex(Size);
	int Iteration = Size - 1;
	for (int i = 0; i < Size; i++)
		VectorOfVertex[i].weight = MAX;
	VectorOfVertex[Iteration].weight = 0;
	std::vector<int> LeftIndexes, RightIndexes;
	std::set<int> Mixer;
	RightIndexes.push_back(Iteration);
	int OptReshLength = 0;
	int counter = 6; // отвечает за крайний левый индекс для стд =3, для ленты = 6
	while (RightIndexes.size() != 0) // МБ КОГДА НЕТ ПРАВЫХ вершин
		/* Левые индексы являются входными для правых индексов, в конце каждой итерации левый индексы становятся правыми */
	{
		LeftIndexes.clear();
		// тут ищем левые индесы, которые являются входными для правых
		for(unsigned k = 0; k < RightIndexes.size(); k++)
			for(int i = 0; i < Size; i++) // ищем входяшие вершины
				if (TableOfRoutes[i][RightIndexes[k]] != 0 && Iteration > i)
					LeftIndexes.push_back(i);

		//mix it with set(delete duplicates)
		for (unsigned i = 0; i < LeftIndexes.size(); i++)	
			Mixer.insert(LeftIndexes[i]);
		LeftIndexes.assign(Mixer.begin(), Mixer.end());
		Mixer.clear();

		std::cout << " x" << counter << " | d(x" << counter << ", x" << counter + 1 << ")  ";
		std::cout << std::setw(OptReshLength * 13) << "";
	        std::cout << "| Оптимальное решение \n"; 
		std::cout << std::setw(4) << " " << "|";	
/*		for(unsigned i = 0; i < RightIndexes.size(); i++)  // Обратный порядок вывода индексов
			std::cout << std::setw(5) << "X" << counter + 1 << "=" << RightIndexes[i] + 1 << std::setw(5) << "|";
*/		 
		for (int i = RightIndexes.size() - 1; i >= 0; i--)
			std::cout << std::setw(5) << "X" << counter + 1 << "=" << RightIndexes[i] + 1 << std::setw(5) << "|";

		std::cout << "f" << counter << "(x" << counter << ")| " << "x" << counter + 1 << "*\n";
//		std::cout << std::setfill('-') << std::setw(40) << "\n";	
//		std::cout << std::setfill(' ');
		std::string OUT;
		for (unsigned i = 0; i < LeftIndexes.size(); i++) {
			std::cout << "  " << LeftIndexes[i] + 1 << " |";
			OptReshLength = LeftIndexes.size() - 1; // вычисляем для отсупа в 45 строке
//			for (unsigned k = 0; k < RightIndexes.size(); k++){ // Обратный порядок обработки правых индексов, то есть с конца
			for (int k = RightIndexes.size() - 1; k >= 0; k--){
				if (TableOfRoutes[RightIndexes[k]][LeftIndexes[i]] == 0){
					OUT = " no_route   |";
					std::cout << std::setw(13) << OUT;
				}
				else{
					OUT = std::to_string(TableOfRoutes[RightIndexes[k]][LeftIndexes[i]]) + "+" 
						  + std::to_string(VectorOfVertex[RightIndexes[k]].weight) + "=" 
						  + std::to_string(VectorOfVertex[RightIndexes[k]].weight 
						  + TableOfRoutes[RightIndexes[k]][LeftIndexes[i]]) + "  |";
					std::cout << std::setw(13) << OUT;
				}
				if (VectorOfVertex[RightIndexes[k]].weight + TableOfRoutes[RightIndexes[k]][LeftIndexes[i]] <
						VectorOfVertex[LeftIndexes[i]].weight && TableOfRoutes[LeftIndexes[i]][RightIndexes[k]] != 0
				   ) // если путь + правая вершина < левая вершина, то замена 
				{
					VectorOfVertex[LeftIndexes[i]].weight = VectorOfVertex[RightIndexes[k]].weight + TableOfRoutes[RightIndexes[k]][LeftIndexes[i]];
					VectorOfVertex[LeftIndexes[i]].pastVertexIndex = RightIndexes[k];
				}
			}
			std::cout << std::setw(5) << VectorOfVertex[LeftIndexes[i]].weight << " | " 
				<< VectorOfVertex[LeftIndexes[i]].pastVertexIndex + 1 << "\n";
		}

		RightIndexes.clear();
		while (LeftIndexes.size() != 0)
		{
			RightIndexes.push_back(LeftIndexes.back());
			LeftIndexes.pop_back();
		}
		counter--;
		Iteration--;
		std::cout << "\n";
		if (RightIndexes[0] == 0)
			break;
	}
	/* ВЫВОД маршрутной таблицы проблема с весами и пред индексами, т.к. мы решали с конца
	std::cout << "\n вся маршрутная таблица:\n";
	std::cout << "узел   метка\n"; 
	for(unsigned i = 0; i < VectorOfVertex.size(); i++){
		std::cout << std::setw(3) << i + 1 << ")   [" << VectorOfVertex[i].weight << ", " << 
			VectorOfVertex[i].pastVertexIndex + 1 << "]\n";
	}*/

	/* это вывод оптимального маршрута */
	int k = 0, now = VectorOfVertex[k].pastVertexIndex;
	std::cout << "Оптимальный маршрут:\n1 -> ";
	while (now + 1 != 7){
		std::cout << now + 1 << " -> ";
		k = now;
		now = VectorOfVertex[k].pastVertexIndex;
	}
	std::cout << '7';
}

void bandedInput(int** TableOfRoutes, int Size){
	int randomMaxElement = 100;
	srand(time(NULL));
	TableOfRoutes[0][1] = rand() % randomMaxElement;
	TableOfRoutes[0][3] = rand() % randomMaxElement;
	for (int i = 1; i < Size; i++)
		for (int j = i + 1; j < i + 4; j++)
			TableOfRoutes[i][j] = rand() % randomMaxElement;// заполняем "ленту" рандомными числами
	for (int i = 1; i < Size; i++)
		for (int j = 0; j < i; j++)
			TableOfRoutes[i][j] = TableOfRoutes[j][i];
}
void exin(int** TableOfRoutes){
	TableOfRoutes[0][1] = TableOfRoutes[1][0] = 7;
	TableOfRoutes[0][2] = TableOfRoutes[2][0] = 8;
	TableOfRoutes[0][3] = TableOfRoutes[3][0]= 5;
	TableOfRoutes[1][4] = TableOfRoutes[4][1] = 12;
	TableOfRoutes[2][4] = TableOfRoutes[4][2] = 8;
	TableOfRoutes[2][5] = TableOfRoutes[5][2] =9;
	TableOfRoutes[3][4] = TableOfRoutes[4][3] =7;
	TableOfRoutes[3][5] = TableOfRoutes[5][3] =13;
	TableOfRoutes[4][6] = TableOfRoutes[6][4] =9;
	TableOfRoutes[5][6] = TableOfRoutes[6][5] =6;
}

void MatrixOutput(int** a, int n){
	std::cout << "\n  ";
	for (int i = 0; i < n; i++ )
		std::cout << std::setw(3) << i + 1;
	std::cout << "\n\n";
	for (int i = 0; i < n; i++ )
	{
		std::cout << i + 1 << " ";
		for (int j = 0; j < n; j++)
			if (a[i][j] == 0)
				std::cout << std::setw(3) << "-";
			else
				std::cout << std::setw(3) << a[i][j];
		std::cout << "\n";	
	}
	std::cout << "\n";
}

int main(){
	setlocale(LC_ALL, "");
	int n = 7;
	int** TableOfRoutes = new int*[n];
	for (int i = 0; i < n; i++)
		TableOfRoutes[i] = new int[n];
	for (int i = 0; i < n; i++)
		for ( int j = 0; j < n; j++)
			TableOfRoutes[i][j] = 0;
	bandedInput(TableOfRoutes, n);
//	exin(TableOfRoutes);	// пример из методы
	MatrixOutput(TableOfRoutes, n);
	
	dynprFunc(TableOfRoutes, n);	

	std::cout << "\n";
	for (int i = 0; i < n; i++)
		delete[] TableOfRoutes[i];
	delete[] TableOfRoutes;
	return 0;
}
