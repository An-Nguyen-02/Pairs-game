#ifndef CARD_HH
#define CARD_HH
#include <QPushButton>
#include <QIcon>
#include <QPixmap>
enum Visibility_type {OPEN, HIDDEN, EMPTY};

class Card
{
public:
    // Init for temporary flip up card
    Card();
    Card(QPushButton* button);

    void set_front(QPixmap front_face);
    void set_back(QPixmap back_face);
    void set_visibility(Visibility_type visibility);
    QPixmap get_front();
    QPixmap get_back();
    Visibility_type get_visibility() const;
    QPushButton* get_button();

    void turn_up();
    void turn_down();
    void remove_from_game_board();
    void delete_card();


private:
    QPushButton* button_;
    QPixmap front_face_;
    QPixmap back_face_;
    Visibility_type visibility_;

};

#endif // CARD_HH
