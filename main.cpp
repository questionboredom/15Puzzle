#include "SFML/Graphics.hpp"
#include <cassert>
//#include "Grid.h"
#include "Puzzle.h"
#include "UiPuzzle.h"
#include "UiPuzzle2.h"
#include "Timer.h"
#include <iostream>
#include <string>
#include <algorithm>
sf::Text getCenteredText(const sf::Font& font, const sf::String& string, 
    const sf::Vector2f& boxSize, unsigned int characterSize = 30u)
{
    sf::Text text{string,font, characterSize};
    sf::FloatRect bounds{text.getLocalBounds()};
    text.setOrigin(
        (bounds.width - boxSize.x) * 0.5f + bounds.left, 
        (bounds.height - boxSize.y) * 0.5f + bounds.top
    );
    return text;
}
void consoleApp()
{
    Puzzle<4,4> puzzle{4*4*4*4};
    int tryNumber{ 0 };
    do
    {
        std::cout << "try number: " << tryNumber << '\n';
        for (int r{ 1 }; r <= 4; ++r)
        {
            for (int c{ 1 }; c <= 4; ++c)
            {
                std::cout << puzzle.at(r, c) << ' ';
                if (puzzle.at(r, c) < 10) std::cout << ' ';
            }
            std::cout << '\n';
;       }
        std::string s{};
        do
        {
            std::cin >> s;
        } while (!puzzle.move(std::stoi(s)));
        ++tryNumber;
    }
    while (!puzzle.isSolved());
    
}
namespace SfmlApp
{
    constexpr unsigned int defaultWindowSizeX{ 800 };
    constexpr unsigned int defaultWindowSizeY{ 900 };
    enum LabelEnum
    {
        le_timeSeconds = 1,
        le_movesNumber = 3,
        le_newGame = 4,
        le_labelCount = 5
    };
    struct UiObjects
    {
        std::array<sf::Text, 4> labels{};
        UiPuzzle<4, 4> puzzle;
    };
    std::array<sf::Text, le_labelCount> getLabels(const sf::Font& font)
    {
        std::array<sf::Text, le_labelCount> labels{};

        labels.at(0) = getCenteredText(font, "TIME(s)", { 0,60 }, 60);
        labels.at(0).setPosition(defaultWindowSizeX - 350, 0);
        labels.at(le_timeSeconds) = getCenteredText(font, "0", { 0,30 }, 30);
        labels.at(le_timeSeconds).setPosition(labels.at(0).getPosition()+sf::Vector2f{0,60});

        labels.at(2) = getCenteredText(font, "MOVES", {200,60},60);
        labels.at(2).setPosition(defaultWindowSizeX - 200, 0);
        labels.at(le_movesNumber) = getCenteredText(font, "0", { 200,30 }, 30);
        labels.at(le_movesNumber).setPosition(labels.at(2).getPosition() + sf::Vector2f{0, 60});

        labels.at(le_newGame) = getCenteredText(font, "NEW GAME", { 320,100, }, 60);
        return labels;
    }
    bool puzzleMovedEvent(const sf::Event& event, sf::RenderWindow& window ,
        auto& puzzle, int& tryNumber, sf::Clock& clock, sf::Text& movesLabel)
    {
        if (tryNumber == 0)
            clock = sf::Clock{};
        if (puzzle.movedEvent(event, window))
        {
            ++tryNumber;
            movesLabel.setString(std::to_string(tryNumber));
            return true;
        }
        return false;
    }
    template <unsigned int rows, unsigned int columns>
    UiPuzzle<rows, columns> initPuzzle(const sf::Font& font)
    {
        return UiPuzzle<rows, columns> {{0, 100}, defaultWindowSizeX / columns, font, rows*columns*rows*columns};
    }
    template <unsigned int rows, unsigned int columns>
    UiPuzzle2<rows, columns,NumberTile> initPuzzle2(const sf::Font& font)
    {
        return UiPuzzle2<rows, columns, NumberTile> {sf::Vector2f{0, 100}, defaultWindowSizeX / columns, font, rows * columns * rows * columns};
    }
    template <unsigned int rows, unsigned int columns>
    UiPuzzle2<rows, columns, ImageTile> initPuzzle2(const sf::Texture& texture)
    {
        return UiPuzzle2<rows, columns, ImageTile> {sf::Vector2f{0, 100}, defaultWindowSizeX / columns, &texture, rows * columns * rows * columns};
    }
    template <unsigned int rows, unsigned int columns>
    bool newGameEvent(const sf::Event& event, const sf::Font& font, UiPuzzle<rows,columns>& puzzle,
        int& tryNumber, std::array<sf::Text, le_labelCount>& labels, sf::Clock& clock)
    {
        if (!labels.at(le_newGame).getLocalBounds().contains(
            static_cast<float>(event.mouseButton.x), 
            static_cast<float>(event.mouseButton.y))
            ) return false;
        puzzle = initPuzzle<rows,columns>(font);
        tryNumber = 0;
        clock = sf::Clock{};
        labels = getLabels(font);
        return true;
    }
    template <unsigned int rows, unsigned int columns, class Tile>
    bool newGameEvent(const sf::Event& event, const sf::Font& font, UiPuzzle2<rows, columns, Tile>& puzzle,
        int& tryNumber, std::array<sf::Text, le_labelCount>& labels, sf::Clock& clock)
    {
        if (!labels.at(le_newGame).getLocalBounds().contains(
            static_cast<float>(event.mouseButton.x),
            static_cast<float>(event.mouseButton.y))
            ) return false;
        puzzle.randomisePuzzle(rows * columns * rows * columns);
        tryNumber = 0;
        clock = sf::Clock{};
        labels = getLabels(font);
        return true;
    }
    void handleEvents(sf::RenderWindow& window, std::array<sf::Text, le_labelCount>& labels, auto& puzzle, int& tryNumber,
        sf::Clock& clock, const sf::Font& font)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
            {
                sf::Vector2u defaultWindowSize{defaultWindowSizeX, defaultWindowSizeY};
                window.setSize(defaultWindowSize);
            }
            newGameEvent(event, font, puzzle, tryNumber, labels, clock);
            if (puzzle.isSolved()) break;
            puzzleMovedEvent(event, window, puzzle, tryNumber, clock, labels.at(le_movesNumber));
            labels.at(le_timeSeconds).setString(
                std::to_string(
                    static_cast<unsigned int>(clock.getElapsedTime().asSeconds())
                )
            );
        }
    }
    template <unsigned int rows, unsigned int columns>
    void run()
    {
        sf::Font font{}; int tryNumber{};
        if (!font.loadFromFile("CascadiaCode.ttf"))
        {
            //std::cout << "cannot load font" << '\n';
            return;
        }
        std::array<sf::Text, le_labelCount> labels{getLabels(font)};
        UiPuzzle<rows, columns> puzzle{initPuzzle<rows,columns>(font)};
        sf::RenderWindow window(sf::VideoMode(defaultWindowSizeX, defaultWindowSizeY), "SFML works!");
        sf::Clock clock{};
        while (window.isOpen())
        {
            handleEvents(window, labels, puzzle, tryNumber, clock, font);
            window.clear();
            for (auto& label : labels) window.draw(label);
            window.draw(puzzle);
            window.display();
        }
    }
    template <unsigned int rows, unsigned int columns>
    void run2()
    {
        sf::Font font{}; sf::Texture texture{}; int tryNumber{};
        if (!font.loadFromFile("CascadiaCode.ttf")||!texture.loadFromFile("squareIcon.jpg"))
        {
            //std::cout << "cannot load font" << '\n';
            return;
        }
        std::array<sf::Text, le_labelCount> labels{getLabels(font)};
        UiPuzzle2<rows, columns, ImageTile> puzzle{initPuzzle2<rows,columns>(texture)};
        sf::RenderWindow window(sf::VideoMode(defaultWindowSizeX, defaultWindowSizeY), 
            std::to_string(rows).append("x").append(std::to_string(columns).append(" puzzle")));
        sf::Clock clock;
        while (window.isOpen())
        {
            handleEvents(window, labels, puzzle, tryNumber, clock, font);
            window.clear();
            std::for_each(labels.begin(), labels.end(), [&window](auto& label) {window.draw(label); });
            //for (auto& label : labels) window.draw(label);
            window.draw(puzzle);
            window.display();
        }
    }
}
int main()
{
    
    SfmlApp::run2<5,5>();
    //consoleApp();
    return 0;
}