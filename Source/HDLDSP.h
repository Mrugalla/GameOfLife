#pragma once

/*
todo:
	feld in eigene komponente, parameter dadrunter
	audio-funktionalität / schnittstelle bauen
*/

namespace HDL {
	enum class Cell { Dead, Alive };
	enum class Topology { Normal, Torus }; // unlimited field

	class MetaGrid {
	public:
		MetaGrid() {
			makeNeighborsArray();
		}

		void setGrid(const int& blox, const Rectangle<int>& boundsP) {
			auto reset = false;
			if (bounds != boundsP) {
				bounds = boundsP;
				widthInv = 1.f / float(bounds.getWidth());
				heightInv = 1.f / float(bounds.getHeight());
				reset = true;
			}
			if (blocks != blox) {
				blocks = blox;
				cells.resize(blocks);
				for (auto& s : cells)
					s.resize(blocks);
				cellsT = cells;
				reset = true;
			}

			if (reset)
				grid.setGrid(blocks, blocks, bounds);
		}

		void setTopology(const Topology& t) { topology = t; }

		void draw(Graphics& g, const Colour& c) {
			grid.draw(g, c);
			
			for (auto x = 0; x < blocks; ++x)
				for (auto y = 0; y < blocks; ++y)
					if (cells[x][y] == Cell::Alive)
						g.fillRect(grid.getRect(x, y, 1, 1).reduced(2));
		}

		void onClick(const MouseEvent& evt) {
			auto position = evt.getPosition().toFloat();
			auto relX = position.getX() * widthInv;
			auto relY = position.getY() * heightInv;

			auto x = int(relX * blocks);
			auto y = int(relY * blocks);

			if(cells[x][y] == Cell::Alive)
				cells[x][y] = Cell::Dead;
			else
				cells[x][y] = Cell::Alive;
		}

		void onUpdate() {
			for(auto x = 0; x < blocks; ++x)
				for (auto y = 0; y < blocks; ++y)
					applyRules(x, y, getAliveNeighborsCount(x, y));
			cells = cellsT;
		}

	private:
		void makeNeighborsArray() {
			neighbors[0].setXY(-1, -1);
			neighbors[1].setXY( 0, -1);
			neighbors[2].setXY( 1, -1);
			neighbors[3].setXY(-1,  0);
			neighbors[4].setXY( 1,  0);
			neighbors[5].setXY(-1,  1);
			neighbors[6].setXY( 0,  1);
			neighbors[7].setXY( 1,  1);
		}

		const int getAliveNeighborsCount(const int& x, const int& y) {
			auto count = 0;
			
			switch (topology) {
			case Topology::Normal:
				for (auto& n : neighbors) {
					if (count == 4)
						break;
					auto xx = n.getX() + x;
					auto yy = n.getY() + y;
					if (inRange(xx) && inRange(yy))
						if (cells[xx][yy] == Cell::Alive)
							++count;
				}
				break;
			case Topology::Torus:
				for (auto& n : neighbors) {
					if (count == 4)
						break;
					auto xx = n.getX() + x;
					auto yy = n.getY() + y;
					if (xx < 0)
						xx += blocks;
					else if (!(xx < blocks))
						xx -= blocks;
					if (yy < 0)
						yy += blocks;
					else if (!(yy < blocks))
						yy -= blocks;
					
					if (cells[xx][yy] == Cell::Alive)
						++count;
				}
				break;
			}
			return count;
		}

		const bool inRange(const int& x) const {
			if (x > -1 && x < blocks)
				return true;
			return false;
		}

		void applyRules(const int& x, const int& y, const int& count) {
			if (cells[x][y] == Cell::Dead) {
				if (count == 3)
					cellsT[x][y] = Cell::Alive;
				else
					cellsT[x][y] = Cell::Dead;
			}
			else {
				if (count < 2 || count > 3)
					cellsT[x][y] = Cell::Dead;
				else
					cellsT[x][y] = Cell::Alive;
			}
		}

		Grid grid;
		Topology topology{ Topology::Torus };
		int blocks{ -1 };
		Rectangle<int> bounds;
		float widthInv, heightInv;

		std::vector<std::vector<Cell>> cells, cellsT;
		Point<int> neighbors[8];
	};
}