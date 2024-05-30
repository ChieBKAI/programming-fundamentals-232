#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

#define CALC_REQ_EXP(r, c) (r * 257 + c * 139 + 89) % 900 + 1



const Position Position::npos = Position(-1, -1);
Position::Position(int r, int c) : r(r), c(c) {}
Position::Position(const string &str_pos) {
    this->r = stoi(str_pos.substr(1, str_pos.find(',') - 1));
    this->c = stoi(str_pos.substr(str_pos.find(',') + 1, str_pos.length() - str_pos.find(',') - 2));

}
int Position::getRow() const { return r; }
int Position::getCol() const { return c; }
void Position::setRow(int r) { this->r = r; }
void Position::setCol(int c) { this->c = c; }
string Position::str() const {
    return "(" + to_string(r) + "," + to_string(c) + ")";
}
bool Position::isEqual(int in_r, int in_c) const { return r == in_r && c == in_c; }
bool Position::isEqual(const Position &pos) const { return r == pos.r && c == pos.c; }
bool Position::operator==(const Position &pos) const { return r == pos.r && c == pos.c; }
int Position::manhattanDistance(const Position &pos1, const Position &pos2) {
    return abs(pos1.r - pos2.r) + abs(pos1.c - pos2.c);
}




MapElement::MapElement(ElementType in_type) : type(in_type) {}
MapElement::~MapElement() {}
ElementType MapElement::getType() const { return type; }


Path::Path() : MapElement(PATH) {}
Wall::Wall() : MapElement(WALL) {}
FakeWall::FakeWall(int in_req_exp) : MapElement(FAKE_WALL), req_exp(in_req_exp) {}
int FakeWall::getReqExp() const { return req_exp; }






Map::Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls) : num_rows(num_rows), num_cols(num_cols) {
    map = new MapElement**[num_rows];
    for (int i = 0; i < num_rows; i++) {
        map[i] = new MapElement*[num_cols];
        for (int j = 0; j < num_cols; j++) {
            map[i][j] = new Path();
        }
    }
    for (int i = 0; i < num_walls; i++) {
        int row = array_walls[i].getRow();
        int col = array_walls[i].getCol();
        map[row][col] = new Wall();
    }
    for (int i = 0; i < num_fake_walls; i++) {
        int row = array_fake_walls[i].getRow();
        int col = array_fake_walls[i].getCol();
        map[row][col] = new FakeWall(CALC_REQ_EXP(row, col));
    }
}
Map::~Map() {
    for (int i = num_rows - 1; i >= 0; i--) {
        for (int j = num_cols - 1; j >= 0; j--) {
            delete map[i][j];
        }
        delete[] map[i];
    }
    delete[] map;
}
bool Map::isValid(const Position &pos, MovingObject *mv_obj) const {
    if (pos.getRow() < 0 || pos.getRow() >= num_rows || pos.getCol() < 0 || pos.getCol() >= num_cols) {
        return false;
    }
    if (map[pos.getRow()][pos.getCol()]->getType() == WALL) {
        return false;
    }
    return true;
}
bool Map::isFakeWall(const Position &pos) const {
    if (map[pos.getRow()][pos.getCol()]->getType() == FAKE_WALL) {
        return true;
    }
    return false;
}
int Map::get_req_exp(const Position &pos) const {
    if (map[pos.getRow()][pos.getCol()]->getType() == FAKE_WALL) {
        return dynamic_cast<FakeWall*>(map[pos.getRow()][pos.getCol()])->getReqExp();
    }
    return 0;
}
int Map::getNumRows() const { return num_rows; }
int Map::getNumCols() const { return num_cols; }






void check_stats (int &HP, int &EXP) {
    if (HP < 0) {HP =  0;}
    if (HP > 500) {HP = 500;}
    if (EXP < 0) {EXP = 0;}
    if (EXP > 900) {EXP = 900;}
}





MovingObject::MovingObject(int index, const Position pos, Map *map, const string &name) : index(index), pos(pos), map(map), name(name) {}
MovingObject::~MovingObject() {}
Position MovingObject::getCurrentPosition() const { return pos; }
string MovingObject::getName() const { return name; }






Character::Character(int index, const Position &init_pos, Map *map, const string &name) : MovingObject(index, init_pos, map, name) {}
Character::~Character() {}
int Character::getHP() const { return hp; }
int Character::getEXP() const { return exp; }
void Character::setHP(int in_hp) {
    hp = in_hp;
    check_stats(hp, exp);
}
void Character::setEXP(int in_exp) {
    exp = in_exp;
    check_stats(hp, exp);
}
CharacterType Character::getType() const { return type; }




Sherlock::Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp) : Character(index, init_pos, map, "Sherlock"), moving_rule(moving_rule), moving_rule_index(0) {
    hp = init_hp;
    exp = init_exp;
    check_stats(hp, exp);
    type = SHERLOCK;
    bag = new SherlockBag(this);
}
Sherlock::~Sherlock() {
    delete bag;
}

Position Sherlock::getNextPosition() {
    Position next_pos = pos;
    if (moving_rule_index >= moving_rule.length()) {
        moving_rule_index = 0;
    }
    if (moving_rule[moving_rule_index] == 'U') {
        next_pos.setRow(next_pos.getRow() - 1);
    } else if (moving_rule[moving_rule_index] == 'D') {
        next_pos.setRow(next_pos.getRow() + 1);
    } else if (moving_rule[moving_rule_index] == 'L') {
        next_pos.setCol(next_pos.getCol() - 1);
    } else if (moving_rule[moving_rule_index] == 'R') {
        next_pos.setCol(next_pos.getCol() + 1);
    }

    if (!map->isValid(next_pos, this)) {
        return Position::npos;
    }
    return next_pos;
}

void Sherlock::move() {
    Position next_pos = getNextPosition();
    moving_rule_index++;
    if (next_pos.isEqual(Position::npos.getRow(), Position::npos.getCol())) {
        return;
    }
    pos = next_pos;
}

string Sherlock::str() const {
    return "Sherlock[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}

void Sherlock::useItem(BaseItem *item, Robot *robot) {
    if (item->canUse(this, robot)) {
        item->use(this, robot);
    }
}

bool Sherlock::useItemBefore() {
    BaseItem* item = bag->get(EXCEMPTION_CARD);
    if (item != nullptr) {
        useItem(item);
        return true;
    }
    return false;
}

bool Sherlock::useItemAfter() {
    BaseItem* item = bag->getFirst();
    while (item != nullptr) {
        if (item->getType() != EXCEMPTION_CARD && item->getType() != PASSING_CARD) {
            useItem(item);
            bag->get(item->getType());
            return true;
        }
        item = item->getNext();
    }
    return false;
}

void Sherlock::givePassingCard(BaseBag* WatsonBag) {
    BaseItem* item = bag->get(PASSING_CARD);
    while (item != nullptr) {
        WatsonBag->insert(item);
        item = bag->get(PASSING_CARD);
    }
}

Watson::Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp) : Character(index, init_pos, map, "Watson"), moving_rule(moving_rule), moving_rule_index(0) {
    hp = init_hp;
    exp = init_exp;
    check_stats(hp, exp);
    type = WATSON;
    bag = new WatsonBag(this);
}
Watson::~Watson() {
    delete bag;
}
Position Watson::getNextPosition() {
    if (moving_rule[0] == '\0') { return Position::npos; }

    Position next_pos = pos;
    if (moving_rule_index >= moving_rule.length()) {
        moving_rule_index = 0;
    }
    if (moving_rule[moving_rule_index] == 'U') {
        next_pos.setRow(next_pos.getRow() - 1);
    } else if (moving_rule[moving_rule_index] == 'D') {
        next_pos.setRow(next_pos.getRow() + 1);
    } else if (moving_rule[moving_rule_index] == 'L') {
        next_pos.setCol(next_pos.getCol() - 1);
    } else if (moving_rule[moving_rule_index] == 'R') {
        next_pos.setCol(next_pos.getCol() + 1);
    }
    if (!map->isValid(next_pos, this)) {
        return Position::npos;
    }
    return next_pos;
}


void Watson::move() {
    Position next_pos = getNextPosition();
    moving_rule_index++;
    if (next_pos == Position::npos) {
        return;
    }
    if (map->isFakeWall(next_pos)) {
        if (exp <= map->get_req_exp(next_pos)) {
            return;
        }
    }
    pos = next_pos;
    
}

string Watson::str() const {
    return "Watson[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}

void Watson::useItem(BaseItem *item, Robot *robot) {
    if (item->canUse(this, robot)) {
        item->use(this, robot);
    }
}

void Watson::giveExemptionCard(BaseBag* SherlockBag) {
    BaseItem* item = bag->get(EXCEMPTION_CARD);
    while (item != nullptr) {
        SherlockBag->insert(item);
        item = bag->get(EXCEMPTION_CARD);
    }
}

bool Watson::useItemBefore(Robot *robot) {
    BaseItem* item = bag->get(PASSING_CARD);
    if (item != nullptr) {
        useItem(item, robot);
        return true;
    }
    return false;
}

bool Watson::useItemAfter() {
    BaseItem* item = bag->getFirst();
    while (item != nullptr) {
        if (item->getType() != EXCEMPTION_CARD && item->getType() != PASSING_CARD) {
            useItem(item);
            bag->get(item->getType());
            return true;
        }
        item = item->getNext();
    }
    return false;
}

Criminal::Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson) : 
    Character(index, init_pos, map, "Criminal"), sherlock(sherlock), watson(watson) {
    type = CRIMINAL;
    past_pos = Position::npos;
    move_count = 0;
    isFirstRobot = true;
}
Criminal::~Criminal() {}
Position Criminal::get_max_distance_pos() { //priority U > L > D > R
    int max_distance = 0;
    int distance;
    Position max_pos = pos;
    for (int i = 0; i < 4; i++) {
        Position next_pos = pos;
        if (i == 0) {
            next_pos.setRow(next_pos.getRow() - 1);
        } else if (i == 1) {
            next_pos.setCol(next_pos.getCol() - 1);
        } else if (i == 2) {
            next_pos.setRow(next_pos.getRow() + 1);
        } else if (i == 3) {
            next_pos.setCol(next_pos.getCol() + 1);
        }
        distance = Position::manhattanDistance(next_pos, sherlock->getCurrentPosition());
        if (distance > max_distance && map->isValid(next_pos, this)) {
            max_distance = distance;
            max_pos = next_pos;
        }
    }
    return max_pos;
}

Position Criminal::getNextPosition() {
    return get_max_distance_pos();
}

Position Criminal::getPastPosition() {
    return past_pos;
}

void Criminal::move() {
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this)) {
        past_pos = pos;
        pos = next_pos;
        move_count++;
    }
}

int Criminal::getMoveCount() {
    return move_count;
}

string Criminal::str() const {
    return "Criminal[index=" + to_string(index) + ";pos=" + pos.str() + "]";
}





///////// ROBOT //////////

Robot::Robot(int index, const Position &init_pos, Map *map, RobotType type) : MovingObject(index, init_pos, map, "Robot"), type(type) {
    int i = init_pos.getRow();
    int j = init_pos.getCol();
    int s = sumOfDigits(i * j);
    if (0 <= s && s <= 1) {
        item = new MagicBook();
    } else if (2 <= s && s <= 3) {
        item = new EnergyDrink();
    } else if (4 <= s && s <= 5) {
        item = new FirstAid();
    } else if (6 <= s && s <= 7) {
        item = new ExcemptionCard();
    } else if (8 <= s && s <= 9) {
        item = new PassingCard();
        int t = (i * 11 + j) % 4;
        if (t == 0) {
            dynamic_cast<PassingCard*>(item)->setChallenge("RobotS");
        } else if (t == 1) {
            dynamic_cast<PassingCard*>(item)->setChallenge("RobotC");
        } else if (t == 2) {
            dynamic_cast<PassingCard*>(item)->setChallenge("RobotSW");
        } else if (t == 3) {
            dynamic_cast<PassingCard*>(item)->setChallenge("all");
        }
    }
}
Robot::~Robot() {
    delete item;
}
int Robot::sumOfDigits(int n) {
    int sum = 0;
    while (n != 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}
Position Robot::getCurrentPosition() const { return pos; }
RobotType Robot::getType() const { return type; }
int Robot::getDistance(Character *character) const {
    return Position::manhattanDistance(pos, character->getCurrentPosition());
}
int Robot::getDistance() const { return 0; }
BaseItem* Robot::giveItem() {
    BaseItem *result = item;
    return result;
}

///////// ROBOT C //////////

RobotC::RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal) : Robot(index, init_pos, map, C), criminal(criminal) {}
RobotC::~RobotC() {}
Position RobotC::getNextPosition() {
    return criminal->getPastPosition();
}
void RobotC::move() {
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this)) {
        pos = next_pos;
    }
}
string RobotC::str() const {
    return "Robot[pos=" + pos.str() + ";type=C;dist=" + "]";
}


///////// ROBOT S //////////

RobotS::RobotS(int index, const Position &init_pos, Map *map, Criminal* criminal, Sherlock *sherlock) : Robot(index, init_pos, map, S), sherlock(sherlock), criminal(criminal) {}
RobotS::~RobotS() {}
Position RobotS::getNextPosition() { //min distance to sherlock
    int min_distance = getDistance();
    int distance;
    Position min_pos = pos;
    for (int i = 0; i < 4; i++) { //priority U > R > D > L
        Position next_pos = pos;
        if (i == 0) {
            next_pos.setRow(next_pos.getRow() - 1);
        } else if (i == 1) {
            next_pos.setCol(next_pos.getCol() + 1);
        } else if (i == 2) {
            next_pos.setRow(next_pos.getRow() + 1);
        } else if (i == 3) {
            next_pos.setCol(next_pos.getCol() - 1);
        }
        distance = Position::manhattanDistance(next_pos, sherlock->getCurrentPosition());
        if (distance < min_distance && map->isValid(next_pos, this)) {
            min_distance = distance;
            min_pos = next_pos;
        }
    }
    if (min_pos.isEqual(pos)) {
        return Position::npos;
    }
    return min_pos;
}
void RobotS::move() {
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this)) {
        pos = next_pos;
    }
}
int RobotS::getDistance() const {
    return Robot::getDistance(this->sherlock);
}
string RobotS::str() const {
    return "Robot[pos=" + pos.str() + ";type=S;dist=" + to_string(getDistance()) + "]";
}



///////// ROBOT W //////////

RobotW::RobotW(int index, const Position &init_pos, Map *map, Criminal* criminal, Watson *watson) : Robot(index, init_pos, map, W), watson(watson), criminal(criminal) {}
RobotW::~RobotW() {}
Position RobotW::getNextPosition() {
    int min_distance = getDistance();
    int distance;
    Position min_pos = pos;
    for (int i = 0; i < 4; i++) { //priority U > R > D > L
        Position next_pos = pos;
        if (i == 0) {
            next_pos.setRow(next_pos.getRow() - 1);
        } else if (i == 1) {
            next_pos.setCol(next_pos.getCol() + 1);
        } else if (i == 2) {
            next_pos.setRow(next_pos.getRow() + 1);
        } else if (i == 3) {
            next_pos.setCol(next_pos.getCol() - 1);
        }
        distance = Position::manhattanDistance(next_pos, watson->getCurrentPosition());
        if (distance < min_distance && map->isValid(next_pos, this)) {
            min_distance = distance;
            min_pos = next_pos;
        }
    }
    if (min_pos.isEqual(pos)) {
        return Position::npos;
    }
    return min_pos;
}
void RobotW::move() {
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this)) {
        pos = next_pos;
    }
}
int RobotW::getDistance() const {
    return Robot::getDistance(this->watson);
}
string RobotW::str() const {
    return "Robot[pos=" + pos.str() + ";type=W;dist=" + to_string(getDistance()) + "]";
}



///////// ROBOT SW //////////

RobotSW::RobotSW(int index, const Position &init_pos, Map *map, Criminal* criminal, Sherlock *sherlock, Watson *watson) : Robot(index, init_pos, map, SW), criminal(criminal), sherlock(sherlock), watson(watson) {}
RobotSW::~RobotSW() {}
Position RobotSW::getNextPosition() { // 8 directions priority: U > UR > R > DR > D > DL > L > UL
    int min_distance = 9999999;
    int distance;
    Position min_pos = pos;
    for (int i = 0; i < 8; i++) {
        Position next_pos = pos;
        if (i == 0) {
            next_pos.setRow(next_pos.getRow() - 2);
        } else if (i == 1) {
            next_pos.setRow(next_pos.getRow() - 1);
            next_pos.setCol(next_pos.getCol() + 1);
        } else if (i == 2) {
            next_pos.setCol(next_pos.getCol() + 2);
        } else if (i == 3) {
            next_pos.setRow(next_pos.getRow() + 1);
            next_pos.setCol(next_pos.getCol() + 1);
        } else if (i == 4) {
            next_pos.setRow(next_pos.getRow() + 2);
        } else if (i == 5) {
            next_pos.setRow(next_pos.getRow() + 1);
            next_pos.setCol(next_pos.getCol() - 1);
        } else if (i == 6) {
            next_pos.setCol(next_pos.getCol() - 2);
        } else if (i == 7) {
            next_pos.setRow(next_pos.getRow() - 1);
            next_pos.setCol(next_pos.getCol() - 1);
        }
        distance = Position::manhattanDistance(next_pos, sherlock->getCurrentPosition()) + Position::manhattanDistance(next_pos, watson->getCurrentPosition());
        if (distance < min_distance && map->isValid(next_pos, this)) {
            min_distance = distance;
            min_pos = next_pos;
        }
    }
    if (min_pos.isEqual(pos)) {
        return Position::npos;
    }
    return min_pos;
    
}
void RobotSW::move() {
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this)) {
        pos = next_pos;
    }
}
int RobotSW::getDistance() const {
    return Robot::getDistance(this->sherlock) + Robot::getDistance(this->watson);
}
string RobotSW::str() const {
    return "Robot[pos=" + pos.str() + ";type=SW;dist=" + to_string(getDistance()) + "]";
}





///////// BASE ITEM //////////
BaseItem::BaseItem(ItemType in_type) : type(in_type), next(nullptr) {}
BaseItem::~BaseItem() {}
BaseItem* BaseItem::getNext() { return next; }
ItemType BaseItem::getType() const { return type; }
void BaseItem::setNext(BaseItem *in_next) { next = in_next; }


///////// EXEMPTION CARD //////////
ExcemptionCard::ExcemptionCard() : BaseItem(EXCEMPTION_CARD) {}
bool ExcemptionCard::canUse(Character *character, Robot *robot) {
    if (character->getType() == SHERLOCK && character->getEXP() % 2 != 0) {
        return true;
    }
    return false;
}
void ExcemptionCard::use(Character *character, Robot *robot) {
    // nothing
}

///////// PASSING CARD //////////

PassingCard::PassingCard() : BaseItem(PASSING_CARD) {}
PassingCard::PassingCard(const string &in_challenge) : BaseItem(PASSING_CARD), challenge(in_challenge) {}
bool PassingCard::canUse(Character *character, Robot *robot) {
    if (character->getType() == WATSON && character->getHP() % 2 == 0) {
        return true;
    }
    return false;
}
void PassingCard::use(Character *character, Robot *robot) {
    if (!(challenge == "all" ||
        (challenge == "RobotS" && robot->getType() == S) ||
        (challenge == "RobotC" && robot->getType() == C) ||
        (challenge == "RobotSW" && robot->getType() == SW))) {
            character->setEXP(character->getEXP() - 50);
    }
}
void PassingCard::setChallenge(const string &in_challenge) { challenge = in_challenge; }


///////// MAGIC BOOK //////////
MagicBook::MagicBook() : BaseItem(MAGIC_BOOK) {}
bool MagicBook::canUse(Character *character, Robot *robot) {
    if (character->getEXP() <= 350) {
        return true;
    }
    return false;
}
void MagicBook::use(Character *character, Robot *robot) {
    character->setEXP(character->getEXP() * 1.25);
}


///////// ENERGY DRINK //////////
EnergyDrink::EnergyDrink() : BaseItem(ENERGY_DRINK) {}
bool EnergyDrink::canUse(Character *character, Robot *robot) {
    if (character->getHP() <= 100) {
        return true;
    }
    return false;
}
void EnergyDrink::use(Character *character, Robot *robot) {
    character->setHP(character->getHP() * 1.20);
}


///////// FIRST AID //////////
FirstAid::FirstAid() : BaseItem(FIRST_AID) {}
bool FirstAid::canUse(Character *character, Robot *robot) {
    if (character->getHP() <= 100 || character->getEXP() <= 350) {
        return true;
    }
    return false;
}
void FirstAid::use(Character *character, Robot *robot) {
    character->setHP(character->getHP() * 1.50);
}



///////// BASE BAG //////////
BaseBag::BaseBag(int capacity) : capacity(capacity), count(0), first_item(nullptr) {}
BaseBag::~BaseBag() {}
bool BaseBag::insert(BaseItem *in_item) {
    if (count >= capacity) {
        return false;
    }
    if (first_item == nullptr) {
        first_item = in_item;
    } else {
        BaseItem *current = first_item;
        while (current->getNext() != nullptr) {
            current = current->getNext();
        }
        current->setNext(in_item);
    }
    count++;
    return true;
}
BaseItem* BaseBag::get() {
    if (first_item == nullptr) {
        return nullptr;
    }
    BaseItem *result = first_item;
    first_item = first_item->getNext();
    count--;
    return result;
}
BaseItem* BaseBag::get(ItemType type) {
    if (first_item == nullptr) {
        return nullptr;
    }
    BaseItem *current = first_item;
    BaseItem *prev = nullptr;
    while (current != nullptr) {
        if (current->getType() == type) {
            if (prev == nullptr) {
                first_item = current->getNext();
            } else { //swap with first_item
                prev->setNext(first_item);
                first_item->setNext(current->getNext());
                first_item = current;
                current = get();
            }
            count--;
            return current;
        }
        prev = current;
        current = current->getNext();
    }
    return nullptr;
}
string BaseBag::str() const {
    string result = "Bag[count=" + to_string(count);
    BaseItem *current = first_item;
    while (current != nullptr) {
        result += ";" + to_string(current->getType());
        current = current->getNext();
    }
    if (count == 0) {
        result += ";";
    }
    result += "]";
    return result;
}
int BaseBag::size() const { return count; }
BaseItem* BaseBag::getFirst() { return first_item; }
int BaseBag::getCount() const { return count; }


///////// SHERLOCK BAG AND WATSON BAG //////////
SherlockBag::SherlockBag(Sherlock* sherlock) : BaseBag(capacity) {
    sherlock->bag = this;
}
WatsonBag::WatsonBag(Watson* watson) : BaseBag(capacity) {
    watson->bag = this;
}



///////// ARRAY MOVING OBJECT //////////
ArrayMovingObject::ArrayMovingObject(int capacity) : capacity(capacity) {
    arr_mv_objs = new MovingObject*[capacity];
    count = 0;
}
ArrayMovingObject::~ArrayMovingObject() {
    delete[] arr_mv_objs;
}
bool ArrayMovingObject::isFull() const { return count >= capacity; }
bool ArrayMovingObject::add(MovingObject *mv_obj) {
    if (count < capacity) {
        arr_mv_objs[count] = mv_obj;
        count++;
        return true;
    }
    return false;
}
MovingObject* ArrayMovingObject::get(int index) {
    if (index >= 0 && index < count) {
        return arr_mv_objs[index];
    }
    return nullptr;
}
int ArrayMovingObject::size() const { return count; }
string ArrayMovingObject::str() const {
    string result = "ArrayMovingObject[count=" + to_string(count) + ";capacity=" + to_string(capacity);
    for (int i = 0; i < count; i++) {
        result += ";" + arr_mv_objs[i]->str();
    }
    if (count == 0) {
        result += ";";
    }
    result += "]";
    return result;
}



///////// CONFIGURATION //////////

Configuration::Configuration(const string & filepath) {
    ifstream file(filepath);
    if (file.is_open()) {
        while (!file.eof()) {
            string line;
            getline(file, line);
            string command = line.substr(0, line.find('='));
            string value = line.substr(line.find('=') + 1);

            if (command == "MAP_NUM_ROWS") {
                map_num_rows = stoi(value);
            } else if (command == "MAP_NUM_COLS") {
                map_num_cols = stoi(value);
            } else if (command == "MAX_NUM_MOVING_OBJECTS"){
                max_num_moving_objects = stoi(value);
            } else if (command == "ARRAY_WALLS") {
                num_walls = 0;
                for (int i = 0; i < line.length(); i++) {
                    if (line[i] == ',') {num_walls++;}
                }
                arr_walls = new Position[num_walls];
                
                value.erase(0, 1);
                value.erase(value.length() - 1, 1);
                stringstream ss(value);
                int i = 0;
                while(getline(ss, value, ';')) {
                    arr_walls[i] = Position(value);
                    i++;
                }
            } else if (command == "ARRAY_FAKE_WALLS") {
                num_fake_walls = 0;
                for (int i = 0; i < line.length(); i++) {
                    if (line[i] == ',') {num_fake_walls++;}
                }
                if (num_fake_walls == 0) { continue;}
                arr_fake_walls = new Position[num_fake_walls];
                
                value.erase(0, 1);
                value.erase(value.length() - 1, 1);
                stringstream ss(value);
                int i = 0;
                while(getline(ss, value, ';')) {
                    arr_fake_walls[i] = Position(value);
                    i++;
                }
            } else if (command == "SHERLOCK_MOVING_RULE") {
                sherlock_moving_rule = value;
            } else if (command == "SHERLOCK_INIT_POS") {
                sherlock_init_pos = Position(value);
            } else if (command == "SHERLOCK_INIT_HP") {
                sherlock_init_hp = stoi(value);
            } else if (command == "SHERLOCK_INIT_EXP") {
                sherlock_init_exp = stoi(value);
            } else if (command == "WATSON_MOVING_RULE") {
                watson_moving_rule = value;
            } else if (command == "WATSON_INIT_POS") {
                watson_init_pos = Position(value);
            } else if (command == "WATSON_INIT_HP") {
                watson_init_hp = stoi(value);
            } else if (command == "WATSON_INIT_EXP") {
                watson_init_exp = stoi(value);
            } else if (command == "CRIMINAL_INIT_POS") {
                criminal_init_pos = Position(value);
            } else if (command == "NUM_STEPS") {
                num_steps = stoi(value);
            }
        }
    }
}
Configuration::~Configuration() {
    delete[] arr_walls;
    delete[] arr_fake_walls;
}
string Configuration::str() const {
    string str = "Configuration[\n";
    str += "MAP_NUM_ROWS=" + to_string(map_num_rows) + "\n";
    str += "MAP_NUM_COLS=" + to_string(map_num_cols) + "\n";
    str += "MAX_NUM_MOVING_OBJECTS=" + to_string(max_num_moving_objects) + "\n";
    str += "NUM_WALLS=" + to_string(num_walls) + "\n";
    str += "ARRAY_WALLS=[";
    for (int i = 0; i < num_walls; i++) {
        str += arr_walls[i].str();
        if (i < num_walls - 1) {
            str += ";";
        }
    }
    str += "]\n";
    str += "NUM_FAKE_WALLS=" + to_string(num_fake_walls) + "\n";
    str += "ARRAY_FAKE_WALLS=[";
    for (int i = 0; i < num_fake_walls; i++) {
        str += arr_fake_walls[i].str();
        if (i < num_fake_walls - 1) {
            str += ",";
        }
    }
    str += "]\n";
    str += "SHERLOCK_MOVING_RULE=" + sherlock_moving_rule + "\n";
    str += "SHERLOCK_INIT_POS=" + sherlock_init_pos.str() + "\n";
    str += "SHERLOCK_INIT_HP=" + to_string(sherlock_init_hp) + "\n";
    str += "SHERLOCK_INIT_EXP=" + to_string(sherlock_init_exp) + "\n";
    str += "WATSON_MOVING_RULE=" + watson_moving_rule + "\n";
    str += "WATSON_INIT_POS=" + watson_init_pos.str() + "\n";
    str += "WATSON_INIT_HP=" + to_string(watson_init_hp) + "\n";
    str += "WATSON_INIT_EXP=" + to_string(watson_init_exp) + "\n";
    str += "CRIMINAL_INIT_POS=" + criminal_init_pos.str() + "\n";
    str += "NUM_STEPS=" + to_string(num_steps) + "\n";
    str += "]";

    return str;
}



///////// STUDY PINK PROGRAM //////////

StudyPinkProgram::StudyPinkProgram(const string & config_file_path) {
    config = new Configuration(config_file_path);
    map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
    sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
    watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
    criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);
    arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
    arr_mv_objs->add(sherlock);
    arr_mv_objs->add(watson);
    arr_mv_objs->add(criminal);
    stop = false;
}

bool StudyPinkProgram::isStop() const {
    if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()) ||
        watson->getCurrentPosition().isEqual(criminal->getCurrentPosition()) ||
        sherlock->getHP() <= 0 || watson->getHP() <= 0 ||
        sherlock->getEXP() <= 0 || watson->getEXP() <= 0){
            return true;
    }
    return false;
}

void StudyPinkProgram::printResult() const {
    if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
        cout << "Sherlock caught the criminal" << endl;
    }
    else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
        cout << "Watson caught the criminal" << endl;
    }
    else {
        cout << "The criminal escaped" << endl;
    }
}

void StudyPinkProgram::printStep(int si) const {
    cout << "Step: " << setw(4) << setfill('0') << si
        << "--"
        << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
}

void StudyPinkProgram::createRobot() {
    if (criminal->isFirstRobot) { // create a RobotC
        RobotC *robotC = new RobotC(arr_mv_objs->size(), criminal->getPastPosition(), map, criminal);
        arr_mv_objs->add(robotC);
        criminal->isFirstRobot = false;
    } else {
        int distanceToSherlock = Position::manhattanDistance(criminal->getPastPosition(), sherlock->getCurrentPosition());
        int distanceToWatson = Position::manhattanDistance(criminal->getPastPosition(), watson->getCurrentPosition());
        if (distanceToSherlock < distanceToWatson) { // create a RobotS
            RobotS *robotS = new RobotS(arr_mv_objs->size(), criminal->getPastPosition(), map, criminal, sherlock);
            arr_mv_objs->add(robotS);
        } else if (distanceToSherlock > distanceToWatson) { // create a RobotW
            RobotW *robotW = new RobotW(arr_mv_objs->size(), criminal->getPastPosition(), map, criminal, watson);
            arr_mv_objs->add(robotW);
        } else { // create a RobotSW
            RobotSW *robotSW = new RobotSW(arr_mv_objs->size(), criminal->getPastPosition(), map, criminal, sherlock, watson);
            arr_mv_objs->add(robotSW);
        }
    }
    criminal->move_count = 0;
}

bool StudyPinkProgram::isMeet(MovingObject *mv_obj1, MovingObject *mv_obj2) {
    return mv_obj1->getCurrentPosition().isEqual(mv_obj2->getCurrentPosition());
}

void StudyPinkProgram::exchangeItem() {
    sherlock->givePassingCard(watson->bag);
    watson->giveExemptionCard(sherlock->bag);
}


void StudyPinkProgram::runInStep() {
    // Sherlock meet Watson, they will exchange items
    if (isMeet(sherlock, watson)) {
        exchangeItem();
    }

    // Sherlock meet the robot
    for (int i = 0; i < arr_mv_objs->size(); i++) {
        if (isMeet(sherlock, arr_mv_objs->get(i)) && arr_mv_objs->get(i)->getName() == "Robot") {
            Robot *robot = dynamic_cast<Robot*>(arr_mv_objs->get(i));
            RobotType robot_type = robot->getType();

            sherlock->useItemBefore();
            
            if (robot_type == S) {
                if (sherlock->getEXP() > 400) {
                    sherlock->bag->insert(robot->giveItem());
                } else {
                    sherlock->setEXP(sherlock->getEXP() * 0.90);
                }
            } else if (robot_type == W) {
                sherlock->bag->insert(robot->giveItem());
            } else if (robot_type == SW) {
                if (sherlock->getEXP() > 300 && sherlock->getHP() > 335) {
                    sherlock->bag->insert(robot->giveItem());
                } else {
                    sherlock->setEXP(sherlock->getEXP() * 0.85);
                    sherlock->setHP(sherlock->getHP() * 0.85);
                }
            } else if (robot_type == C) {
                if (sherlock->getEXP() > 500) {
                    sherlock->pos = criminal->getCurrentPosition(); // catch the criminal
                } else {
                    sherlock->bag->insert(robot->giveItem());
                }
            }

            sherlock->useItemAfter();
        }
    }

    // Watson meet the robot
    for (int i = 0; i < arr_mv_objs->size(); i++) {
        if (isMeet(watson, arr_mv_objs->get(i)) && arr_mv_objs->get(i)->getName() == "Robot") {
            Robot *robot = dynamic_cast<Robot*>(arr_mv_objs->get(i));
            RobotType robot_type = robot->getType();

            watson->useItemBefore(robot);
            
            if (robot_type == S) {
                // nothing
            } else if (robot_type == W) {
                if (watson->getHP() > 350) {
                    watson->bag->insert(robot->giveItem());
                } else {
                    watson->setHP(watson->getHP() * 0.95);
                }
            } else if (robot_type == SW) {
                if (watson->getEXP() > 600 && watson->getHP() > 165) {
                    watson->bag->insert(robot->giveItem());
                } else {
                    watson->setEXP(watson->getEXP() * 0.85);
                    watson->setHP(watson->getHP() * 0.85);
                }
            } else if (robot_type == C) {
                watson->bag->insert(robot->giveItem());
            }

            watson->useItemAfter();
        }
    }

    // Check the stop condition
    if (isStop()) { 
        stop = true;
        return;
    }

    // Create a new Robot whenever the criminal moves 3 times
    if (criminal->getMoveCount() == 3) {
        createRobot();
        arr_mv_objs->get(arr_mv_objs->size() - 1)->move(); // move the newly created robot
    }
}

void StudyPinkProgram::run(bool verbose) {
    int si = 0;
    
    
    while (!stop && si < config->num_steps) {
        for (int i = 0; i < arr_mv_objs->size(); i++) {
            arr_mv_objs->get(i)->move();
            runInStep();
        }
        if (verbose) { printStep(si); }

        si++;
    }


    if (verbose) { printStep(si);}
    printResult();
}

StudyPinkProgram::~StudyPinkProgram() {
    delete config;
    delete map;
    delete sherlock;
    delete watson;
    delete criminal;
    delete arr_mv_objs;
}



////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////