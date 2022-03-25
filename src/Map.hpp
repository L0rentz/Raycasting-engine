#ifndef MAP_HPP_
#define MAP_HPP_

#include <fstream>
#include <vector>

#include "Exception.hpp"
#include "SFML/Graphics.hpp"

class Map {
    public:
        Map(const sf::RenderWindow &window);
        Map(const std::string &path, const sf::RenderWindow &window);
        ~Map();

        void draw(sf::RenderWindow &window);
        void event(const sf::Event &event, const sf::RenderWindow &window);

    protected:
    private:
        typedef struct s_cell {
            bool exist;
            sf::RectangleShape rect;
            bool isEdge[4];
            int edgeId[4];
        } cell_t;

        typedef struct s_line {
            sf::Vector2f A;
            sf::Vector2f B;
        } line_t;

        enum {
            TOP,
            BOTTOM,
            LEFT,
            RIGHT
        };
        
        sf::Vector2u _windowSize;
        float _caseSize;
        char *_filemap;
        cell_t *_cellmap;
        int _cellmapSize;
        int _cellmapWidth;
        std::map<int, line_t> _vertexmap;
        sf::CircleShape _vertice;
        sf::VertexArray _line;

        void generateCellmap();
        void constructorCommons(const sf::RenderWindow &window);
        void generateVertexmap();
        int getCellId(const sf::Vector2f &position) const;
        void getFileContent(const std::string &path);
        void drawVertexmap(sf::RenderWindow &window);
};

#endif /* !MAP_HPP_ */
