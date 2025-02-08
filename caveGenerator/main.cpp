#include <SFML/Graphics.hpp>
#include <random>
#include <vector>
#include <iostream>
#include <cmath>
#include <limits>

struct edge {
    sf::Vector2f from{}; // x1 и y1
    sf::Vector2f to{}; // x2 и y2
    int weight;
    size_t from_room_index; // Индекс комнаты "откуда"
    size_t to_room_index;   // Индекс комнаты "куда"
};

struct Room {
    //Левая верхняя точка
    float x_1; float y_1;
    //Правая верхняя точка
    float x_2; float y_2;
    //Размер сторон
    float x_s; float y_s;

};

static std::vector<Room> Delenie(sf::RenderWindow& window, float minSize, int min_rooms) {
    std::vector<Room> rooms;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(40, 60);
    // 1. Начальный прямоугольник
    Room initialRoom = {
        10.00,
        10.00,
        window.getSize().x - 10.00,
        window.getSize().y - 10.00,
        window.getSize().x - 20.00,
        window.getSize().y - 20.00,
    };
    rooms.push_back(initialRoom);

    while (min_rooms > rooms.size()) {
        Room Room1{};
        Room Room2{};
        std::vector<Room> tmp{};
        for (int i = 0; i < rooms.size(); i++) {
            float naKakoyPros = float(distrib(gen)) / 100;
            float max_storona = std::max(rooms[i].x_s, rooms[i].y_s);
            bool isHorisontiol = true;
            if (max_storona != rooms[i].x_s) {
                isHorisontiol = false;
            }
            if (isHorisontiol) {  //если сторона х больше
                if (minSize * 0.85 <= rooms[i].x_s * naKakoyPros) {
                    Room1 = {
                        rooms[i].x_1,
                        rooms[i].y_1,
                        rooms[i].x_1 + rooms[i].x_s * naKakoyPros,
                        rooms[i].y_2,
                        (rooms[i].x_2 - rooms[i].x_1) * naKakoyPros,
                        rooms[i].y_2 - rooms[i].y_1

                    };

                    Room2 = {
                        rooms[i].x_1 + rooms[i].x_s * naKakoyPros,
                        rooms[i].y_1,
                        rooms[i].x_2,
                        rooms[i].y_2,
                        (rooms[i].x_2 - rooms[i].x_1) - (rooms[i].x_2 - rooms[i].x_1) * naKakoyPros,
                        rooms[i].y_2 - rooms[i].y_1
                    };

                }
                else {
                    continue;
                }


            }
            else if (minSize * 0.85 <= rooms[i].y_s * naKakoyPros) { // если сторона у больше
                Room1 = {
                    rooms[i].x_1,
                    rooms[i].y_1,
                    rooms[i].x_2,
                    rooms[i].y_1 + (rooms[i].y_2 - rooms[i].y_1) * naKakoyPros,
                    rooms[i].x_2 - rooms[i].x_1,
                    (rooms[i].y_2 - rooms[i].y_1) * naKakoyPros
                };
                Room2 = {
                    rooms[i].x_1,
                    rooms[i].y_1 + (rooms[i].y_2 - rooms[i].y_1) * naKakoyPros,
                    rooms[i].x_2,
                    rooms[i].y_2,
                    rooms[i].x_2 - rooms[i].x_1,
                    (rooms[i].y_2 - rooms[i].y_1) - (rooms[i].y_2 - rooms[i].y_1) * naKakoyPros
                };


            }
            else {
                continue;
            }
            rooms.erase(rooms.begin() + i);
            tmp.push_back(Room1);
            tmp.push_back(Room2);
        }
        for (int i = 0; i < tmp.size(); i++) {
            rooms.push_back(tmp[i]);
        }

    }




    return rooms;
}

static std::vector<Room> RoomGenerator(std::vector <Room> spaceForRooms) {

    std::vector <Room> rooms;

    std::vector <Room> dels = spaceForRooms;

    for (const auto& del : dels) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib1(15, 20);

        float naKakoyProc = float(distrib1(gen)) / 100;

        Room room = {
            del.x_1 + del.x_s * naKakoyProc,
            del.y_1 + del.y_s * naKakoyProc,
            del.x_2 - del.x_s * naKakoyProc,
            del.y_2 - del.y_s * naKakoyProc,
            (del.x_2 - del.x_s * naKakoyProc) - (del.x_1 + del.x_s * naKakoyProc),
            (del.y_2 - del.y_s * naKakoyProc) - (del.y_1 + del.y_s * naKakoyProc)
        };
        rooms.push_back(room);

    }

    return rooms;
}

static std::vector<edge> GrafGenerator(const std::vector<Room>& rooms) {
    std::vector<edge> allEdges; // Все возможные ребра
    for (size_t i = 0; i < rooms.size(); ++i) {
        for (size_t j = i + 1; j < rooms.size(); ++j) {
            bool areNeighbors = false;

            if (abs(rooms[i].y_1 - rooms[j].y_2) < 1 || abs(rooms[i].y_2 - rooms[j].y_1) < 1) {
                if (rooms[i].x_1 <= rooms[j].x_2 && rooms[i].x_2 >= rooms[j].x_1) {
                    areNeighbors = true;
                }
            }
            if (abs(rooms[i].x_1 - rooms[j].x_2) < 1 || abs(rooms[i].x_2 - rooms[j].x_1) < 1) {
                if (rooms[i].y_1 <= rooms[j].y_2 && rooms[i].y_2 >= rooms[j].y_1) {
                    areNeighbors = true;
                }
            }

            if (areNeighbors) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> distrib1(1, 60);

                allEdges.push_back({
                    sf::Vector2f{float(rooms[i].x_1 + rooms[i].x_s * 0.5f), float(rooms[i].y_1 + rooms[i].y_s * 0.5f)},
                    sf::Vector2f{float(rooms[j].x_1 + rooms[j].x_s * 0.5f), float(rooms[j].y_1 + rooms[j].y_s * 0.5f)},
                    distrib1(gen), i, j
                    });
            }
        }
    }

    // Алгоритм Прима
    std::vector<edge> minSpanningTree;
    std::vector<bool> visited(rooms.size(), false);
    std::vector<int> minWeight(rooms.size(), std::numeric_limits<int>::max());
    std::vector<size_t> parent(rooms.size(), -1);

    minWeight[0] = 0; 

    for (int count = 0; count < rooms.size(); ++count) {
        size_t u = -1;
        int min = std::numeric_limits<int>::max();


        for (size_t v = 0; v < rooms.size(); ++v) {
            if (!visited[v] && minWeight[v] < min) {
                min = minWeight[v];
                u = v;
            }
        }

        if (u == -1) break; 

        visited[u] = true;

        
        for (const auto& edge : allEdges) {
            size_t v = -1;
            if (edge.from_room_index == u && !visited[edge.to_room_index]) v = edge.to_room_index;
            if (edge.to_room_index == u && !visited[edge.from_room_index]) v = edge.from_room_index;

            if (v != -1 && edge.weight < minWeight[v]) {
                minWeight[v] = edge.weight;
                parent[v] = u;
            }
        }
    }


    for (size_t i = 1; i < rooms.size(); ++i) {
        for (const auto& edge : allEdges) {
            if ((edge.from_room_index == i && edge.to_room_index == parent[i]) || (edge.to_room_index == i && edge.from_room_index == parent[i])) {
                minSpanningTree.push_back(edge);
                break;
            }
        }
    }

    return minSpanningTree;
}

enum class CellState { Alive, Dead, AlwaysAlive };


// Функция для создания клеточного автомата
static std::vector<std::vector<CellState>> cellularAutomaton(const std::vector<Room>& rooms, const std::vector<edge>& edges, int width, int height, float cellSize) {
    std::vector<std::vector<CellState>> grid(height, std::vector<CellState>(width, CellState::Dead));

    // Заполнение комнат клетками "всегда живыми"
    for (const auto& room : rooms) {
        for (int y = static_cast<int>(room.y_1 / cellSize); y < static_cast<int>(room.y_2 / cellSize); ++y) {
            for (int x = static_cast<int>(room.x_1 / cellSize); x < static_cast<int>(room.x_2 / cellSize); ++x) {
                grid[y][x] = CellState::AlwaysAlive;
            }
        }
    }

    for (const auto& edge : edges) {
        // Рисование коридора между двумя точками (улучшенное, ширина 3 клетки)
        sf::Vector2f dir = edge.to - edge.from;
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        sf::Vector2f unitDir = dir / len;
        sf::Vector2f perpDir = { -unitDir.y, unitDir.x }; // Перпендикулярный вектор


        for (float i = 0; i < len; i += cellSize) {
            sf::Vector2f point = edge.from + unitDir * i;
            for (int j = -1; j <= 1; ++j) { // Итерация по 3 клеткам шириной
                int x = static_cast<int>((point.x + perpDir.x * j) / cellSize);
                int y = static_cast<int>((point.y + perpDir.y * j) / cellSize);
                if (x >= 0 && x < width && y >= 0 && y < height) {
                    grid[y][x] = CellState::AlwaysAlive;
                }
            }
        }
    }

    // Случайное добавление живых клеток вокруг комнат и коридоров (сниженная вероятность)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(0.0, 1.0);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x] == CellState::Dead) {
                if (distrib(gen) < 0.08) { // Снижено до 8% вероятности
                    grid[y][x] = CellState::Alive;
                }
            }
        }
    }

    

    return grid;
}

// Функция для применения правил клеточного автомата (B35678/S5678)
static std::vector<std::vector<CellState>> updateCellularAutomaton(const std::vector<std::vector<CellState>>& grid, int width, int height) {
    std::vector<std::vector<CellState>> newGrid = grid;
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int liveNeighbors = 0;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) continue;
                    if (grid[y + dy][x + dx] == CellState::Alive || grid[y + dy][x + dx] == CellState::AlwaysAlive) {
                        liveNeighbors++;
                    }
                }
            }
            // Применение правил B35678/S5678
            //B345 / S4567

            if (grid[y][x] == CellState::AlwaysAlive) {
                newGrid[y][x] = CellState::AlwaysAlive;
            }

            else if (grid[y][x] == CellState::Alive) { // Правила выживания (S)
                if (liveNeighbors < 5 || liveNeighbors > 8) {
                    newGrid[y][x] = CellState::Dead;
                }
            }
            else { // Правила рождения (B)
                if (liveNeighbors == 3 || liveNeighbors == 5 || liveNeighbors == 6 || liveNeighbors == 7 || liveNeighbors == 8) {
                    newGrid[y][x] = CellState::Alive;
                }
            }
        }
    }
    return newGrid;
}

int main() {

    sf::RenderWindow window(sf::VideoMode(1500, 900), L"Генератор пещер");

    float minSize = 100;
    int min_rooms = 45;
    std::vector<Room> lines = Delenie(window, minSize, min_rooms);
    std::vector<Room> rooms = RoomGenerator(lines);
    std::vector<edge> grafs = GrafGenerator(lines);
    int cellSize = 1;
    int gridWidth = static_cast<int>(window.getSize().x / cellSize);
    int gridHeight = static_cast<int>(window.getSize().y / cellSize);
    std::vector<std::vector<CellState>> grid = cellularAutomaton(rooms, grafs, gridWidth, gridHeight, cellSize);


    bool runningCA = false; // Флаг для управления обновлениями КА

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    runningCA = !runningCA; // Переключение обновлений КА с помощью пробела
                }
            }
        }

        window.clear(sf::Color(128, 128, 128));

        // Отрисовка и обновление клеточного автомата
        if (runningCA) {
            grid = updateCellularAutomaton(grid, gridWidth, gridHeight);
        }

        for (int y = 0; y < gridHeight; ++y) {
            for (int x = 0; x < gridWidth; ++x) {
                sf::RectangleShape rect(sf::Vector2f(cellSize, cellSize));
                rect.setPosition(x * cellSize, y * cellSize);
                if (grid[y][x] == CellState::AlwaysAlive) {
                    rect.setFillColor(sf::Color::White);
                }
                else if (grid[y][x] == CellState::Alive) {
                    rect.setFillColor(sf::Color::White);
                }
                else {
                    rect.setFillColor(sf::Color(150,75,0));
                }
                window.draw(rect);
            }
        }


        window.display();
    }


    return 0;
}