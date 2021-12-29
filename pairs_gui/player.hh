#ifndef PLAYER_HH
#define PLAYER_HH
#include <QPushButton>
#include <card.hh>
#include <QLabel>


class Player
{
public:
    Player(QPushButton* button, int stack_x, int stack_y);
    int get_queu();
    void set_queu(int queu);

    // get the position to put the draw card onto the ui
    int get_current_column();
    // Due to limited space, each row can only have 7 cards
    // Therefore, the card columns must restart
    void reset_current_column();
    QPushButton* get_button();
    int get_stack_size();
    // Player button x,y
    int get_x();
    int get_y();

    void turn_stacks();

    void update_back(QPushButton* card_back);
    void update_front(QLabel* card_front);
    void update_current_column();
    void update_stack_y();
    void delete_player();


private:
    QPushButton* button_;
    int queu_;
    const int x_;
    int y_;
    bool face_down_ = true;
    int card_size = 40;
    int current_column_count = 0;

    // I used 2 different type of QWidget because
    // same widget will overlap and hide each other
    // even in INVISIBLE mode.
    std::vector<QPushButton*> cards_back_;
    std::vector<QLabel*> cards_front_;

    void turn_back_stack(bool visible);
    void turn_front_stack(bool visible);
};

#endif // PLAYER_HH
