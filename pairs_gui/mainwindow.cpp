#include "mainwindow.hh"
#include "ui_mainwindow.h"


const QString GAME_RULE= " ";
const QString UI_EXPLAIN = " ";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->centralwidget->setLayout(new QVBoxLayout);
    // Limit the pairs due to limit of card images
        ui->pairs_Box->setRange(1, 51);
        // Limit the player because of limited Gui space
        ui->players_Box->setRange(1, 9);
        ui->minutes_lcdNumber->setStyleSheet("background-color: #2F329F");
        ui->seconds_lcdNumber->setStyleSheet("background-color: #8C329F");
        connect(timer, SIGNAL(timeout()), this, SLOT(start_timer()));
        connect(ui->apply_Button, SIGNAL(clicked()), this, SLOT(apply_button_clicked()));
        connect(ui->reset_Button, SIGNAL(clicked()), this, SLOT(reset_button_clicked()));
        init_card_faces_and_back();
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::init_card_faces_and_back()
{
    card_back = QPixmap("Resources/back.png");
    std::string card_head = "Resources/card_faces/back_";
    std::string card_tail = ".png";
    for (unsigned int i=1; i < 52; i++)
    {
        std::string current_card = card_head + std::to_string(i) + card_tail;
        QPixmap current_face(QString::fromStdString(current_card));
        card_face_list_.push_back(QString::fromStdString(current_card));
    }
    // 51 cards
}

void MainWindow::apply_button_clicked()
{
    QString Qseed = ui->seed_value->text();
    seed_ = stoi_with_check(Qseed.toUtf8().constData());
    // if seed value is blank or invalid then send an error.
    if (seed_ == 0)
    {
        seed_ = time(NULL);
    }

    num_players_ = ui->players_Box->value();
    pairs_ = ui->pairs_Box->value();
    // if number of players is smaller than number of given names
    // the number of names will be the number of players
    process_names();
    timer->start(1000);
    ui->apply_Button->setEnabled(false);
    init_board();
    init_player();





}

unsigned int MainWindow::stoi_with_check(const std::string &str)
{
    bool is_numeric = true;
    if (str.length() == 0)
    {
        return 0;
    }
    for(unsigned int i = 0; i < str.length(); ++i)
    {
        if(not isdigit(str.at(i)))
        {
            is_numeric = false;
            break;
        }
    }
    if(is_numeric)
    {
        return stoi(str);
    }
    else
    {
        return 0;
    }
}

void MainWindow::process_names()
{
    QString Qtext = ui->names_textEdit->toPlainText();
    std::string text = Qtext.toUtf8().constData();
    std::istringstream stream(text);
    std::string line;
    while (std::getline(stream, line))
    {
        names_list_.push_back(line);
    }
    // if number of players is bigger than given names
    // the extra players get default name Player 1, 2,...
    if (names_list_.size() < num_players_)
    {
        int delta = num_players_ - names_list_.size();
        int count = 1;
        for (int i = 1; i < delta+1; i++)
        {
            std::string current_name = "Player " + std::to_string(count);
            names_list_.push_back(current_name);
            count ++ ;
        }
    }
    else if (names_list_.size() > num_players_)
    {
        num_players_ = names_list_.size();
    }
}

void MainWindow::init_board()
{
    unsigned int smaller_factor;
    unsigned int bigger_factor;
    calculate_factors(smaller_factor, bigger_factor);
    QIcon back_icon(card_back);
    // Init the support board (for cursor local position)
    // and Init with EMPTY for the Card
    for (unsigned int i = 0; i < smaller_factor; i++)
    {
        for (unsigned int a = 0; a < bigger_factor; a++)
        {
            QPushButton* pushButton = new QPushButton(this);
            pushButton->setIcon(back_icon);
            pushButton->setIconSize(card_back.rect().size());
            pushButton->setGeometry(VIEW_X + a * CARD_WIDTH,
                                    VIEW_Y + i* CARD_HEIGTH,
                                    CARD_WIDTH,
                                    CARD_HEIGTH);
            pushButton->show();
            game_board_.push_back(Card(pushButton));
            connect(pushButton, SIGNAL(clicked()),
                    this, SLOT(handle_card_clicked()));
        }

    }
    // Init with Hidden Card
    // Drawing a cell to be filled
    std::default_random_engine randomEng(seed_);
    std::uniform_int_distribution<int> distr(0, game_board_.size() - 1);
    // Wiping out the first random number (that is always the lower bound of the distribution)
    distr(randomEng);

    // If the drawn cell is already filled with a card, next empty cell will be used.
    // (The next empty cell is searched for circularly, see function next_free.)
    for(unsigned int i = 0; i < game_board_.size() - 1; i += 2)
    {
        QString current_face = card_face_list_.at(i/2);
        // Adding two identical cards (pairs) in the game board
        for(unsigned int j = 0; j < 2; ++j)
        {
            unsigned int cell = distr(randomEng);
            cell = next_free(cell);
            game_board_.at(cell).set_back(card_back);
            game_board_.at(cell).set_front(current_face);
            game_board_.at(cell).set_visibility(HIDDEN);
        }
    }
}

void MainWindow::calculate_factors(unsigned int &smaller_factor, unsigned int &bigger_factor)
{
    unsigned int product = pairs_*2;

    for(unsigned int i = 1; i * i <= product; ++i)
    {
        if(product % i == 0)
        {
            smaller_factor = i;
        }
    }
    bigger_factor = product / smaller_factor;
}

unsigned int MainWindow::next_free(unsigned int start)
{
    // Finding out the number of rows and columns of the game board
    unsigned int rows = game_board_.size();


    // Starting from the given value
    for(unsigned int i = start; i < rows; ++i)
    {
        if(game_board_.at(i).get_visibility() == EMPTY) // vaihdettu
        {
            return i;
        }
    }

    // Continuing from the beginning
    for(unsigned int i = 0; i < start; ++i)
    {
        if(game_board_.at(i).get_visibility() == EMPTY)
        {
            return i;
        }
    }

    // You should never reach this
    //std::cout << "No more empty spaces" << std::endl;
    return rows - 1;
}

void MainWindow::init_player()
{
    for (unsigned int i = 0; i < num_players_; i++)
    {
        QString player_name = QString::fromStdString(names_list_.at(i));
        QPushButton* player_button = new QPushButton(player_name, this);
        player_button->move(PLAYER_X + i*PLAYER_STEP, PLAYER_Y);
        player_button->show();
        connect(player_button, SIGNAL(clicked()), this, SLOT(handle_player_clicked()));
        Player new_player = Player(player_button, STACK_X + i*STACK_STEP, STACK_Y);
        player_list_.push_back(new_player);
    }
    set_player_queu();
    // First turn init
    QString first_player_name = player_list_.at(0).get_button()->text();
    ui->turn_textBrowser->setText(first_player_name);
}

void MainWindow::handle_player_clicked()
{
    QPoint local_click_position = find_local_cursor();
    for(unsigned int i = 0; i < player_list_.size(); ++i)
    {
        Player& current_player = player_list_.at(i);
        QPushButton* current_button = current_player.get_button();
        if (current_button->geometry().contains(local_click_position))
            {
                current_player.turn_stacks();
                return; // For efficiency reasons
                        // (only one button can be clicked at a time)
            }
    }
}

void MainWindow::set_player_queu()
{
    for (unsigned int i = 0; i < num_players_; i++)
    {
        player_list_.at(i).set_queu(i);
    }
}

QPoint MainWindow::find_local_cursor()
{
    // Storing global cursor position
    QPoint global_click_position = QCursor::pos();

    // Counting local cursor position, i.e. decreasing
    // Main Window's location from the global one
    int local_x = global_click_position.x() - geometry().x();
    int local_y = global_click_position.y() - geometry().y();
    QPoint local_click_position = QPoint(local_x, local_y);
    return local_click_position;
}

void MainWindow::handle_card_clicked()
{
    QPoint local_click_position = find_local_cursor();
    for(unsigned int i = 0; i < game_board_.size(); ++i)
    {
        Card current_card = game_board_.at(i);
        QPushButton* current_button = current_card.get_button();
        if (current_button->geometry().contains(local_click_position))
            {
                // in case of misclick the third card or the flipped card
                if (face_up_cards == 2 or current_button == pressed_button)
                {
                    return;
                }

                face_up_cards ++;
                pressed_button = current_button;
                flipped_card.push_back(current_card);
                current_card.turn_up();

                if (face_up_cards == 1 )
                {
                    return;
                }

                QTimer::singleShot(look_time,this,SLOT(compare_flipped_card()));


                return; // For efficiency reasons
                        // (only one button can be clicked at a time)
            }
    }


}

void MainWindow::compare_flipped_card()
{
    Card& card1 = flipped_card.at(0);
    Card& card2 = flipped_card.at(1);
    QImage image1 = card1.get_front().toImage();
    QImage image2 = card2.get_front().toImage();
    face_up_cards = 0;
    flipped_card.clear();
    if (image1 == image2)
    {
        cards_out+=2;
        add_card_player(card2);
        card2.remove_from_game_board();
        card1.remove_from_game_board();

        Game_is_over();
        return;
    }
    card1.turn_down();
    card2.turn_down();
    pressed_button = nullptr;
    change_turn();

}

void MainWindow::Game_is_over()
{
    if (cards_out == game_board_.size())
    {

        win_or_draw();
    }
}

void MainWindow::win_or_draw()
{
    unsigned int players_list_len = player_list_.size();
    int largest_pairs = find_largest_pairs();
    int count_winners = 0;
    int winner_number = 0;
    for (unsigned int i = 0; i<players_list_len; ++i)
    {
        Player current_player = player_list_.at(i);
        int current_player_pair = current_player.get_stack_size();

        if (current_player_pair == largest_pairs)
        {
            winner_number = i;
            ++ count_winners;
        }
    }

    if (count_winners == 1)
    {
        QString winner_name = player_list_.at(winner_number).get_button()->text();
        // There is a delay in the code making this code go first the removing card.
        std::string win_str = " has won with " + std::to_string(largest_pairs) + " pairs.";
        QString win_message = winner_name + QString::fromStdString(win_str);
        QMessageBox::information(this, "Congratulations!!!", win_message);
    }
    else
    {
        std::string tie_str = "Tie of " + std::to_string(count_winners) + " players with "
                            + std::to_string(largest_pairs) + " pairs." + " Continue?";
        QString tie_message = QString::fromStdString(tie_str);
        QMessageBox::information(this, "TIE GAME!", tie_message);
    }

    reset_button_clicked();


}

int MainWindow::find_largest_pairs()
{

    unsigned int players_list_len = player_list_.size();
    int largest_pairs = 0;
    for (unsigned int i=0 ; i < players_list_len; ++i)
    {
        Player current = player_list_.at(i);
        int current_player_pair = current.get_stack_size();
        if (current_player_pair > largest_pairs)
        {
            largest_pairs = current_player_pair;
       }
    }
    return largest_pairs;


}

void MainWindow::add_card_player(Card& card)
{
    Player& current_player = player_list_.at(turn);
    int current_column = current_player.get_current_column();
    int x = current_player.get_x();
    int y = current_player.get_y();
    // card are drawn so that it overlap 50% to the previous one.
    draw_player_new_card(x + current_column*(0.5)*CARD_WIDTH, y, card);
    if (current_column == CARD_PER_ROW)
    {
        current_player.update_stack_y();
        current_player.reset_current_column();
        return;
    }
    current_player.update_current_column();
}

void MainWindow::draw_player_new_card(int x, int y, Card &card)
{
    Player& current_player = player_list_.at(turn);

    int card_size = 40;
    // draw 1 card back

    QPushButton* new_card_back = new QPushButton(this);
    new_card_back->setGeometry(x, y, card_size, card_size);
    new_card_back->setIcon(card.get_back());
    new_card_back->setIconSize(card.get_back().rect().size());
    new_card_back->show();
    new_card_back->setVisible(true);
    current_player.update_back(new_card_back);
    // draw 1 card face

    QLabel* new_card_face = new QLabel(this);
    new_card_face->setPixmap(card.get_front());
    new_card_face->setGeometry(x,y,card_size,card_size);
    new_card_face->show();
    new_card_face->setVisible(false);
    current_player.update_front(new_card_face);

}

void MainWindow::change_turn()
{
    if (turn == num_players_-1)
    {
        turn = 0;
    }
    else
    {
        turn++;
    }
    QString player_name = player_list_.at(turn).get_button()->text();
    ui->turn_textBrowser->setText(player_name);
}

void MainWindow::reset_button_clicked()
{

    for (unsigned int i = 0; i <player_list_.size(); i ++)
    {
       Player current_player = player_list_.at(i);

       current_player.delete_player();
    }
    player_list_.clear();

    for (unsigned int i = 0; i < game_board_.size(); i++)
    {
        Card current_card = game_board_.at(i);
        current_card.delete_card();
    }
    game_board_.clear();
    timer->stop();
    ui->seed_value->clear();
    ui->pairs_Box->setValue(1);
    ui->players_Box->setValue(1);
    ui->minutes_lcdNumber->display(0);
    ui->seconds_lcdNumber->display(0);
    ui->turn_textBrowser->clear();
    ui->names_textEdit->clear();
    ui->apply_Button->setEnabled(true);
    turn = 0;
    names_list_.clear();
    flipped_card.clear();
    pressed_button = nullptr;
    cards_out = 0;
    face_up_cards = 0;
    seed_ = 0;
    pairs_ = 0;
    num_players_ = 0;

}

void MainWindow::start_timer()
{
    int sec_value = ui->seconds_lcdNumber->value();
    int min_value = ui->minutes_lcdNumber->value();
    if (sec_value < 60)
    {
        int next_sec = sec_value + 1;
        ui->seconds_lcdNumber->display(next_sec);
    }
    else
    {
        ui->seconds_lcdNumber->display(0);
        int next_min = min_value + 1;
        ui->minutes_lcdNumber->display(next_min);
    }
}
