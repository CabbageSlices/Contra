#ifndef TILEMAP_H_INCLUDED
#define TILEMAP_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

#include <memory>
#include <vector>

class TileMap {

    public:

        TileMap();

        //return all the tiles enclosed in the given region
        //if any part of the region is out of bounds it will only return the tiles that are in boun
        //region coordinates are in world position in pixels
        ///for all regions use region 0,0,0,0 to indicate you want to use entire tilemap
        std::vector<std::shared_ptr<Tile> > getTilesInRegion(const sf::FloatRect& region);

        void draw(sf::RenderWindow& window, const sf::FloatRect& regionToDraw);

    private:

        //take the given region in world coordinates and convert it into grid indices, ensures region is valid
        //left is the indice of hte left edge, width is the indice of the right edge (not width of region)
        //same with top and bottom
        sf::IntRect regionToGridIndices(const sf::FloatRect& region)

        std::vector<std::shared_ptr<Tile> > tiles;
};

#endif // TILEMAP_H_INCLUDED
