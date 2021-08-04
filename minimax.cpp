#include <SFML/Graphics.hpp>
#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;

#define WIN  1000
#define DRAW 0
#define LOSS -1000

#define AI_MARKER     'O'
#define PLAYER_MARKER 'X'
#define EMPTY_SPACE   '-'

#define START_DEPTH 0

#define W_WIDTH 900

const int P_WIDTH  = W_WIDTH / 3;
const float D_SIZE = sqrt(2) * P_WIDTH;
int steps          = 0;
sf::RenderWindow window(sf::VideoMode(W_WIDTH, W_WIDTH), "Tic-Tac-Toe");

void print_game_state(int state) {
    std::string msg;
    if (WIN == state) {
        msg = " PLAYER WIN";
    } else if (DRAW == state) {
        msg = "DRAW";
    } else if (LOSS == state) {
        msg = "PLAYER LOSS";
    }
    sf::Text text;
    sf::Font font;
    font.loadFromFile("Montserrat-Medium.otf");
    text.setFont(font);
    text.setString(msg);
    text.setCharacterSize(32);
    auto size = text.getGlobalBounds();
    text.setPosition(W_WIDTH / 2 - size.width / 2, W_WIDTH / 2 - size.height / 2);
    text.setFillColor(sf::Color::Red);
    text.setOutlineThickness(1);
    text.setOutlineColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);
    window.clear(sf::Color::Cyan);
    window.draw(text);
}

std::vector<std::vector<std::pair<int, int>>> winning_states{{std::make_pair(0, 0), std::make_pair(0, 1), std::make_pair(0, 2)},
                                                             {std::make_pair(1, 0), std::make_pair(1, 1), std::make_pair(1, 2)},
                                                             {std::make_pair(2, 0), std::make_pair(2, 1), std::make_pair(2, 2)},

                                                             {std::make_pair(0, 0), std::make_pair(1, 0), std::make_pair(2, 0)},
                                                             {std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1)},
                                                             {std::make_pair(0, 2), std::make_pair(1, 2), std::make_pair(2, 2)},

                                                             {std::make_pair(0, 0), std::make_pair(1, 1), std::make_pair(2, 2)},
                                                             {std::make_pair(2, 0), std::make_pair(1, 1), std::make_pair(0, 2)}

};
void draw_grid() {
    for (int i = 1; i < 3; i++) {
        sf::VertexArray line(sf::Lines, 2);
        line[0].position = sf::Vector2f(P_WIDTH * i, 0);
        line[0].color    = sf::Color::Black;
        line[1].position = sf::Vector2f(P_WIDTH * i, W_WIDTH);
        line[1].color    = sf::Color::Black;
        window.draw(line);
        line[0].position = sf::Vector2f(0, P_WIDTH * i);
        line[1].position = sf::Vector2f(W_WIDTH, P_WIDTH * i);
        window.draw(line);
    }
}
void draw_x(int posx, int posy) {
    sf::Vector2f vec(P_WIDTH * posx, P_WIDTH * posy);
    sf::RectangleShape line(sf::Vector2f(D_SIZE * 0.9, 8));
    line.setRotation(45);
    line.setPosition(vec.x + D_SIZE * 0.1 / 2 - 4, vec.y + D_SIZE * 0.1 / 2 - 6);
    line.setFillColor(sf::Color::Black);
    window.draw(line);
    line.setRotation(135);
    line.setPosition(vec.x - D_SIZE * 0.1 / 2 + P_WIDTH + 4, vec.y + D_SIZE * 0.1 / 2 - 2);
    window.draw(line);
}
void draw_o(int posx, int posy) {
    sf::Vector2f vec(P_WIDTH * posx, P_WIDTH * posy);
    sf::CircleShape circle(P_WIDTH * 0.9 / 2);
    circle.setOutlineThickness(8);
    circle.setOutlineColor(sf::Color::Black);
    circle.setPosition(vec.x + P_WIDTH * 0.1 / 2, vec.y + P_WIDTH * 0.1 / 2);
    window.draw(circle);
}
void print_in_console(char board[3][3]) {
    cout << endl;
    cout << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << endl;
    cout << "----------" << endl;
    cout << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << endl;
    cout << "----------" << endl;
    cout << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << endl << endl;
}

void print_board(char board[3][3]) {
    window.clear(sf::Color::White);
    draw_grid();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            switch (board[i][j]) {
                case PLAYER_MARKER:
                    draw_x(i, j);
                    break;
                case AI_MARKER:
                    draw_o(i, j);
                    break;
                case EMPTY_SPACE:
                    break;
                default:
                    break;
            }
        }
    }
}

std::vector<std::pair<int, int>> get_legal_moves(char board[3][3]) {
    std::vector<std::pair<int, int>> legal_moves;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != AI_MARKER && board[i][j] != PLAYER_MARKER) {
                legal_moves.push_back(std::make_pair(i, j));
            }
        }
    }

    return legal_moves;
}

bool position_occupied(char board[3][3], std::pair<int, int> pos) {
    auto legal_moves = get_legal_moves(board);

    for (int i = 0; i < legal_moves.size(); i++) {
        if (pos.first == legal_moves[i].first && pos.second == legal_moves[i].second) {
            return false;
        }
    }

    return true;
}

std::vector<std::pair<int, int>> get_occupied_positions(char board[3][3], char marker) {
    std::vector<std::pair<int, int>> occupied_positions;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (marker == board[i][j]) {
                occupied_positions.push_back(std::make_pair(i, j));
            }
        }
    }

    return occupied_positions;
}

bool board_is_full(char board[3][3]) {
    auto legal_moves = get_legal_moves(board);

    if (0 == legal_moves.size()) {
        return true;
    } else {
        return false;
    }
}

bool game_is_won(std::vector<std::pair<int, int>> occupied_positions) {
    bool game_won = false;

    for (int i = 0; i < winning_states.size(); i++) {
        game_won                                        = true;
        std::vector<std::pair<int, int>> curr_win_state = winning_states[i];
        for (int j = 0; j < 3; j++) {
            if (!(std::find(std::begin(occupied_positions), std::end(occupied_positions), curr_win_state[j]) != std::end(occupied_positions))) {
                game_won = false;
                break;
            }
        }

        if (game_won) {
            break;
        }
    }
    return game_won;
}

char get_opponent_marker(char marker) {
    char opponent_marker;
    if (marker == PLAYER_MARKER) {
        opponent_marker = AI_MARKER;
    } else {
        opponent_marker = PLAYER_MARKER;
    }

    return opponent_marker;
}

int get_board_state(char board[3][3], char marker) {
    char opponent_marker = get_opponent_marker(marker);

    auto occupied_positions = get_occupied_positions(board, marker);

    bool is_won = game_is_won(occupied_positions);

    if (is_won) {
        return WIN;
    }

    occupied_positions = get_occupied_positions(board, opponent_marker);
    bool is_lost       = game_is_won(occupied_positions);

    if (is_lost) {
        return LOSS;
    }

    return DRAW;
}

std::pair<int, std::pair<int, int>> minimax_optimization(char board[3][3], char marker, int depth, int alpha, int beta) {
    steps++;
    auto best_move = std::make_pair(-1, -1);
    int best_score = (marker == AI_MARKER) ? LOSS : WIN;

    if (board_is_full(board) || DRAW != get_board_state(board, AI_MARKER)) {
        best_score = get_board_state(board, AI_MARKER);
        return std::make_pair(best_score, best_move);
    }

    auto legal_moves = get_legal_moves(board);

    for (int i = 0; i < legal_moves.size(); i++) {
        auto curr_move                           = legal_moves[i];
        board[curr_move.first][curr_move.second] = marker;

        if (marker == AI_MARKER) {
            int score = minimax_optimization(board, PLAYER_MARKER, depth + 1, alpha, beta).first;

            if (best_score < score) {
                best_score = score - depth * 10;
                best_move  = curr_move;

                alpha                                    = std::max(alpha, best_score);
                board[curr_move.first][curr_move.second] = EMPTY_SPACE;
                if (beta <= alpha) {
                    break;
                }
            }

        } else {
            int score = minimax_optimization(board, AI_MARKER, depth + 1, alpha, beta).first;

            if (best_score > score) {
                best_score = score + depth * 10;
                best_move  = curr_move;

                beta                                     = std::min(beta, best_score);
                board[curr_move.first][curr_move.second] = EMPTY_SPACE;
                if (beta <= alpha) {
                    break;
                }
            }
        }

        board[curr_move.first][curr_move.second] = EMPTY_SPACE;
    }

    return std::make_pair(best_score, best_move);
}

bool game_is_done(char board[3][3]) {
    if (board_is_full(board)) {
        return true;
    }

    if (DRAW != get_board_state(board, AI_MARKER)) {
        return true;
    }

    return false;
}

int main() {
    char board[3][3] = {{EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE}, {EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE}, {EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE}};
    window.clear(sf::Color::White);

    print_board(board);
    window.display();

    while (!game_is_done(board) && window.isOpen()) {
        int row, col;
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                auto pos1 = sf::Mouse::getPosition(window);
                auto pos  = window.mapCoordsToPixel(sf::Vector2f(pos1.x, pos1.y));
                row       = (int)pos.x / P_WIDTH;
                col       = (int)pos.y / P_WIDTH;
                if (position_occupied(board, std::make_pair(row, col))) {
                    break;
                } else {
                    board[row][col] = PLAYER_MARKER;
                }
                print_board(board);
                steps        = 0;
                auto ai_move = minimax_optimization(board, AI_MARKER, START_DEPTH, LOSS, WIN);
                cout << steps << endl;
                cout << ai_move.second.first << " " << ai_move.second.second << endl;
                if (!board_is_full(board)) {
                    board[ai_move.second.first][ai_move.second.second] = AI_MARKER;
                }
                print_board(board);
            }
        }

        window.clear(sf::Color::White);

        print_board(board);
        window.display();
    }
        system("pause");
    if (window.isOpen()) {
        int player_state = get_board_state(board, PLAYER_MARKER);
        print_game_state(player_state);
        window.display();
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed || event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::KeyPressed) {
                    window.close();
                    return 0;
                }
            }
            print_game_state(player_state);
            window.display();
        }
    }
    return 0;
}
