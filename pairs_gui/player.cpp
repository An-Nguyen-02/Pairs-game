#include "player.hh"

Player::Player(QPushButton* button, int stack_x, int stack_y):
    button_(button),
    x_(stack_x),
    y_(stack_y)
{

}

void Player::delete_player()
{
   for (unsigned int i = 0; i < cards_back_.size(); i++)
   {
       delete cards_back_.at(i);
       delete cards_front_.at(i);

   }
   delete button_;
   cards_back_.clear();
   cards_front_.clear();
}

void Player::set_queu(int queu)
{
    queu_ = queu;
}

int Player::get_queu()
{
    return queu_;
}

QPushButton *Player::get_button()
{
    return button_;
}

int Player::get_x()
{
    return x_;
}

int Player::get_y()
{
    return  y_;
}

int Player::get_current_column()
{
    return current_column_count;
}

void Player::update_current_column()
{
    current_column_count++;
}

int Player::get_stack_size()
{
    return cards_back_.size();
}

void Player::update_front(QLabel* card_front)
{
    cards_front_.push_back(card_front);
}

void Player::reset_current_column()
{
    current_column_count = 0;
}

void Player::update_stack_y()
{
    y_+= card_size;
}

void Player::update_back(QPushButton* card_back)
{
    cards_back_.push_back(card_back);
}

void Player::turn_stacks()
{
    turn_back_stack(!face_down_);
    turn_front_stack(face_down_);
    if (face_down_ == true)
    {
        turn_back_stack(!face_down_);
        turn_front_stack(face_down_);
        face_down_ = false;
        return;
    }
    face_down_ = true;
}

void Player::turn_back_stack(bool visible)
{
    for (unsigned int i = 0; i < cards_back_.size(); i++)
    {
       cards_back_.at(i)->setVisible(visible);
    }
}

void Player::turn_front_stack(bool visible)
{
    for (unsigned int i = 0; i < cards_front_.size(); i++)
    {
       cards_front_.at(i)->setVisible(visible);
    }
}
