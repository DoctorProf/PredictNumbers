#include "Headers/NeuralNetWork.h"
#include "Headers/Button.h"
#include "Headers/Cell.h"
#include <numeric>

using namespace sf;

void setStyleText(Text& text, Font& font, size_t sizeCharacter)
{
    text.setFont(font);
    text.setCharacterSize(sizeCharacter);
    text.setFillColor(Color::White);
}
bool bounds(int x) 
{
    return (x > -1 && x < 28);
}
int main() 
{   
    std::vector<std::vector<double>> inputSet;
    std::vector<std::vector<double>> outputSet;
    NeuralNetwork nn(784, { 10, 10 }, 10, 1, 0.6, true);
    //NeuralNetwork nn("Weights.txt");
    RenderWindow window(VideoMode(400, 570), "Predict", sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    Font font;
    font.loadFromFile("C:/Windows/Fonts/Calibri.ttf");  
    Text predict;
    Text number;
    setStyleText(predict, font, 24);
    setStyleText(number, font, 24);
    predict.setPosition(Vector2f(140.0f, 20.0f));
    number.setPosition(Vector2f(260.0f, 50.0f));
    predict.setString(L"Предсказанное число: ");
    number.setString(L"0");
    std::vector<std::vector<Cell>> cells;
    cells.resize(28);
    for (int i = 0; i < 28; i++) 
    {
        for (int j = 0; j < 28; j++) 
        {
            cells[i].push_back(Cell(20 + j * 10, 270 + i * 10));
        }
    }
    std::vector<Button> buttons;
    buttons.push_back(Button(20.0f, 20.0f, 100, 30, L"", font));
    buttons.push_back(Button(20.0f, 70.0f, 100, 30, L"Обучение", font));
    buttons.push_back(Button(20.0f, 120.0f, 100, 30, L"Очистить", font));
    buttons.push_back(Button(20.0f, 170.0f, 100, 30, L"Сохранить сеты", font));
    buttons.push_back(Button(20.0f, 220.0f, 100, 30, L"Очистить сеты", font));
    std::vector<Button> buttonsNum;
    for (int i = 0; i < 10; i++) 
    {
        buttonsNum.push_back(Button(320.0f, 150 + 40.0f * i, 30, 30, std::to_wstring(i), font));
    }
    Clock clock;
    Clock clock2;
    Time accumulatedTime = Time::Zero;
    Time accumulatedTime2 = Time::Zero;
    Time timePerFrame = seconds(1.0f / 60); // tps
    Time timePerFrame2 = seconds(1.0f / 60); // fps
    bool isDrawing = false;
    std::vector<std::string> lines;
    std::string line;
    std::ifstream inputLoad("inputSet.txt");
    while (std::getline(inputLoad, line))
    {
        lines.push_back(line);
    }
    inputLoad.close();
    inputSet.resize(lines.size() / 784);
    for (int i = 0; i < lines.size() / 784; i++) 
    {
        for (int j = 0; j < 784; j++)
        {
            inputSet[i].push_back(std::stod(lines[j + i * 784]));
        }
    }
    lines.clear();
    std::ifstream outputLoad("outputSet.txt");
    while (std::getline(outputLoad, line))
    {
        lines.push_back(line);
    }
    outputLoad.close();
    outputSet.resize(lines.size() / 10);
    for (int i = 0; i < lines.size() / 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            outputSet[i].push_back(std::stod(lines[j + i * 10]));
        }
    }
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                
                window.close();
            }
            else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                isDrawing = true;
            }
            else if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
            {
                isDrawing = false;
            }
            if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
            {
                for (int i = 0; i < buttons.size(); i++) 
                {
                    if (buttons[i].buttonClick(Mouse::getPosition(window).x, Mouse::getPosition(window).y)) 
                    {
                        if (i == 1) 
                        {
                            window.setTitle(L"...ОБУЧЕНИЕ...");
                            window.setTitle(L"Predict");
                            nn.trainToIterarion(inputSet, outputSet, 30, true);
                            window.setTitle(L"ОБУЧЕНИЕ ОКОНЧЕНО");
                        }
                        else if (i == 2)
                        {
                            window.setTitle(L"Predict");
                            for (int j = 0; j < cells.size(); j++)
                            {
                                for (int k = 0; k < cells[j].size(); k++)
                                {
                                    cells[j][k].setColor(0);
                                }
                            }
                            number.setString("0");
                        }
                        else if (i == 3)
                        {
                            std::ofstream inputSave("inputSet.txt");
                            for (int i = 0; i < inputSet.size(); i++)
                            {
                                for (int j = 0; j < inputSet[i].size(); j++)
                                {
                                    inputSave << inputSet[i][j] << "\n";
                                }
                            }
                            inputSave.close();
                            std::ofstream outputSave("outputSet.txt");
                            for (int i = 0; i < outputSet.size(); i++)
                            {
                                for (int j = 0; j < outputSet[i].size(); j++)
                                {
                                    outputSave << outputSet[i][j] << "\n";
                                }
                            }
                            outputSave.close();
                        }
                        else if (i == 4)
                        {
                            inputSet.clear();
                            outputSet.clear();
                            std::ofstream inputSave("inputSet.txt");
                            inputSave.clear();
                            std::ofstream outputSave("outputSet.txt");
                            outputSave.clear();
                        }
                    }
                }
                for (int i = 0; i < buttonsNum.size(); i++) 
                {
                    if (buttonsNum[i].buttonClick(Mouse::getPosition(window).x, Mouse::getPosition(window).y)) 
                    {
                        std::vector<double> set;
                        for (int j = 0; j < cells.size(); j++)
                        {
                            for (int k = 0; k < cells[j].size(); k++)
                            {
                                set.push_back(cells[j][k].getColor());
                            }
                        }
                        inputSet.push_back(set);
                        set.clear();
                        for (int j = 0; j < 10; j++) 
                        {
                            set.push_back(0);
                        }
                        set[i] = 1;
                        outputSet.push_back(set);
                        for (int j = 0; j < cells.size(); j++)
                        {
                            for (int k = 0; k < cells[j].size(); k++)
                            {
                                cells[j][k].setColor(0);
                            }
                        }
                    }
                }
            }
        }
        if (isDrawing) 
        {
            Vector2i coords = Mouse::getPosition(window);
            if ((coords.x >= 20 && coords.x <= 300) && (coords.y >= 270 && coords.y <= 550)) 
            {
                Vector2f coordsVector = Vector2f(std::floor((coords.x - 20) / 10), std::floor((coords.y - 270) / 10));
                if (bounds(coordsVector.x) && bounds(coordsVector.y))
                {
                    cells[coordsVector.y][coordsVector.x].setColor(1);
                    if (bounds(coordsVector.x + 1))
                    {
                        cells[coordsVector.y][coordsVector.x + 1].setColor(cells[coordsVector.y][coordsVector.x + 1].getColor() + 1);
                    }
                    if (bounds(coordsVector.x - 1))
                    {
                        cells[coordsVector.y][coordsVector.x - 1].setColor(cells[coordsVector.y][coordsVector.x - 1].getColor() + 1);
                    }
                    if (bounds(coordsVector.y + 1))
                    {
                        cells[coordsVector.y + 1][coordsVector.x].setColor(cells[coordsVector.y + 1][coordsVector.x].getColor() + 1);
                    }
                    if (bounds(coordsVector.y - 1))
                    {
                        cells[coordsVector.y - 1][coordsVector.x].setColor(cells[coordsVector.y - 1][coordsVector.x].getColor() + 1);
                    }
                }
                std::vector<double> set;
                for (int j = 0; j < cells.size(); j++)
                {
                    for (int k = 0; k < cells[j].size(); k++)
                    {
                        set.push_back(cells[j][k].getColor());
                    }
                }
                nn.predict(set);
                std::vector<Neuron> lastLayers = nn.getLayers()[nn.getLayers().size() - 1];
                std::vector<double> values;
                for (int j = 0; j < lastLayers.size(); j++)
                {
                    values.push_back(lastLayers[j].getValue());
                }
                float sum = 0;
                for (int j = 0; j < values.size(); j++)
                {
                    sum += values[j];
                }
                int num = std::distance(values.begin(), std::max_element(values.begin(), values.end()));
                number.setString(std::to_string(num) + " " + std::to_string((lastLayers[num].getValue() / sum) * 100) + " %");
            }
        }
        accumulatedTime += clock.restart();
        while (accumulatedTime >= timePerFrame)
        {
            buttons[0].setText(L"Кол-во сетов: " + std::to_wstring(inputSet.size()));
            accumulatedTime -= timePerFrame;
        }
        accumulatedTime2 += clock2.restart();
        if (accumulatedTime2 >= timePerFrame2)
        {
            accumulatedTime2 -= timePerFrame2;
            window.clear(Color::Black);
            for (int i = 0; i < cells.size(); i++) 
            {
                for (int j = 0; j < cells[i].size(); j++) 
                {
                    cells[i][j].draw(window);
                }
            }
            for (int i = 0; i < buttons.size(); i++) 
            {
                buttons[i].draw(window);
            }
            for (int i = 0; i < buttonsNum.size(); i++)
            {
                buttonsNum[i].draw(window);
            }
            window.draw(predict);
            window.draw(number);
            window.display();
        }
    }
}