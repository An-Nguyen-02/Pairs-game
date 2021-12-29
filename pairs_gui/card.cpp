#include "card.hh"

Card::Card()
{

}
Card::Card(QPushButton* button):
    button_(button), visibility_(EMPTY)
{

}


QPixmap Card::get_front()
{
    return front_face_;
}

void Card::set_front(QPixmap front_face)
{
    front_face_ = front_face;
}

QPixmap Card::get_back()
{
    return back_face_;
}

void Card::set_back(QPixmap back_face)
{
    back_face_ = back_face;
}

Visibility_type Card::get_visibility() const
{
    return visibility_;
}

void Card::set_visibility(const Visibility_type visibility)
{
    visibility_ = visibility;
}

QPushButton *Card::get_button()
{
    return button_;
}

void Card::turn_up()
{
    visibility_ = OPEN;
    QIcon icon;
    icon.addPixmap(front_face_, QIcon::Normal , QIcon::On);
    button_->setIcon(icon);
    button_->setIconSize(front_face_.rect().size());
    //button_->setEnabled(false);


}

void Card::turn_down()
{
    visibility_ = HIDDEN;
    button_->setIcon(back_face_);
    button_->setIconSize(back_face_.rect().size());
   // button_->setEnabled(true);
}

void Card::remove_from_game_board()
{
    visibility_ = EMPTY;
    button_->setEnabled(false);
    button_->setVisible(false);
}

void Card::delete_card()
{
    delete button_;
}


