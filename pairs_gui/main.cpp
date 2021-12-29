/* Program author
 * Name: Nguyen Nhat An
 * Student number: 50359099
 * User ID: kbanng
 * E-mail: an.nguyen@tuni.fi
 *
 * Notes about the program and its implementation:
 *
 * Desc:
 * This project reimplement the pairs game in week 4 in GUI. The game is best as the most
 * pairs possible (51 pairs- 102 cards) and 5 players (more is possible but not good for the layout).
 * First the player will enter initial value such as seed, number players, number pairs, players names.
 * Then press APPLY button will create the board and the player, if player name is not written,
 * it will be set as default player 1, player 2, etc. Each player will have their own button so that
 * if press it, the card faces that player collected will be shown, click again and card will turn down.
 * The first card flip can be look as long as the player want but the second flipped card will only show for
 * look time constant in mainwindow.hh. When the game end (final message of win or draw is given), if player press OK
 * then the game will reset.
 *
 * Notes:
 * I have arranged the variable, constant, functions in every file in the order that it is easy to read (support functions
 * of a big function should be right below or above the big function).
 *
 * */


#include "mainwindow.hh"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
