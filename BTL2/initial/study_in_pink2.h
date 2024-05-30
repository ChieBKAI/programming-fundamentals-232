/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
class Configuration;
class Map;

class Criminal;
class Character;
class Sherlock;
class Watson;
class Robot;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;




enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD };
enum ElementType { PATH, WALL, FAKE_WALL };
enum RobotType { C, S, W, SW };
enum Stats_Type { HP, EXP };
enum CharacterType { SHERLOCK, WATSON, CRIMINAL };


////////// POSITION //////////////////////
class Position {
public:
    int r, c;


    static const Position npos;
    Position(int r = 0, int c = 0);
    Position(const string& str_pos);
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const;
    bool isEqual(int in_r, int in_c) const;
    bool isEqual(const Position& pos) const;
    static int manhattanDistance(const Position& pos1, const Position& pos2);
    bool operator==(const Position& pos) const;
};

////////////// MAP ELEMENTS /////////////////////////
class MapElement {
protected:
    ElementType type;

public:
    MapElement(ElementType in_type);
    virtual ~MapElement();
    virtual ElementType getType() const;
};

class Path : public MapElement {
public:
    Path();
};

class Wall : public MapElement {
public:
    Wall();
};

class FakeWall : public MapElement {
private:
    int req_exp;

public:
    FakeWall(int in_req_exp);
    int getReqExp() const;
};


////////////// MAP /////////////////////////
class Map {
public:
    int num_rows, num_cols;
    MapElement*** map;


    Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls);
    ~Map();
    bool isValid(const Position& pos, MovingObject* mv_obj) const;
    bool isFakeWall(const Position& pos) const;
    int get_req_exp(const Position& pos) const;
    int getNumRows() const;
    int getNumCols() const;
};


////////////// MOVING OBJECTS /////////////////////////
class MovingObject {
public:
    int index;
    Position pos;
    Map* map;
    string name;
    

    MovingObject(int index, const Position pos, Map* map, const string& name = "");
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    virtual Position getCurrentPosition() const;
    virtual void move() = 0;
    virtual string str() const = 0;
    string getName() const;
};

class Character : public MovingObject {
public:
    int hp;
    int exp;
    CharacterType type;


    Character(int index, const Position& init_pos, Map* map, const string& name);
    virtual ~Character();
    virtual void move() = 0;
    virtual string str() const = 0;
    int getHP() const;
    int getEXP() const;
    void setHP(int in_hp);
    void setEXP(int in_exp);
    CharacterType getType() const;
};

class Sherlock : public Character {
public:
    string moving_rule;
    int moving_rule_index;


    BaseBag* bag;
    Sherlock(int index, const string& moving_rule, const Position& init_pos, Map* map, int init_hp, int init_exp);
    ~Sherlock();
    Position getNextPosition();
    void move();
    string str() const;
    void useItem(BaseItem* item, Robot* robot = nullptr);
    void givePassingCard(BaseBag* WatsonBag);
    bool useItemBefore();
    bool useItemAfter();

};

class Watson : public Character {
public:
    string moving_rule;
    int moving_rule_index;


    BaseBag* bag;
    Watson(int index, const string& moving_rule, const Position& init_pos, Map* map, int init_hp, int init_exp);
    ~Watson();
    Position getNextPosition();
    void move();
    string str() const;
    void useItem(BaseItem* item, Robot* robot = nullptr);
    void giveExemptionCard(BaseBag* SherlockBag);
    bool useItemBefore(Robot* robot);
    bool useItemAfter();
};

class Criminal : public Character {
public:
    Sherlock* sherlock;
    Watson* watson;
    Position past_pos;
    int move_count;
    bool isFirstRobot;


    Criminal(int index, const Position& init_pos, Map* map, Sherlock* sherlock, Watson* watson);
    ~Criminal();
    Position get_max_distance_pos();
    Position getNextPosition();
    Position getPastPosition();
    int getMoveCount();
    void move();
    string str() const;
};

class Robot : public MovingObject {
public:
    RobotType type;
    BaseItem* item;
    int sumOfDigits(int n);


    Robot(int index, const Position& init_pos, Map* map, RobotType type);
    virtual ~Robot();
    Position getCurrentPosition() const;
    virtual void move() = 0;
    virtual int getDistance(Character* character) const;
    virtual int getDistance() const;
    RobotType getType() const;
    BaseItem* giveItem();
};

class RobotC : public Robot {
private:
    Criminal* criminal;

public:
    RobotC(int index, const Position& init_pos, Map* map, Criminal* criminal);
    ~RobotC();
    Position getNextPosition();
    void move();
    string str() const;
};

class RobotS : public Robot {
private:
    Criminal* criminal;
    Sherlock* sherlock;

public:
    RobotS(int index, const Position& init_pos, Map* map, Criminal* criminal, Sherlock* sherlock);
    ~RobotS();
    Position getNextPosition();
    void move();
    int getDistance() const override;
    string str() const;
};

class RobotW : public Robot {
private:
    Criminal* criminal;
    Watson* watson;

public:
    RobotW(int index, const Position& init_pos, Map* map, Criminal* criminal, Watson* watson);
    ~RobotW();
    Position getNextPosition();
    void move();
    int getDistance() const override;
    string str() const;
};

class RobotSW : public Robot {
private:
    Criminal* criminal;
    Sherlock* sherlock;
    Watson* watson;

public:
    RobotSW(int index, const Position& init_pos, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson);
    ~RobotSW();
    Position getNextPosition();
    void move();
    int getDistance() const override;
    string str() const;
};

class BaseItem {
protected:
    ItemType type;
    BaseItem* next;

public:
    BaseItem(ItemType in_type);
    virtual ~BaseItem();
    virtual ItemType getType() const;
    virtual bool canUse(Character *character, Robot *robot) = 0;
    virtual void use(Character *character, Robot *robot) = 0;
    BaseItem* getNext();
    void setNext(BaseItem* in_next);
};

class ExcemptionCard : public BaseItem {
public:
    ExcemptionCard();
    bool canUse(Character *character, Robot *robot);
    void use(Character *character, Robot *robot);
};

class PassingCard : public BaseItem {
public:
    string challenge;


    PassingCard();
    PassingCard(const string& in_challenge);
    bool canUse(Character *character, Robot *robot);
    void use(Character *character, Robot *robot);
    void setChallenge(const string& in_challenge);
};

class MagicBook : public BaseItem {
public:
    MagicBook();
    bool canUse(Character *character, Robot *robot);
    void use(Character *character, Robot *robot);
};

class EnergyDrink : public BaseItem {
public:
    EnergyDrink();
    bool canUse(Character *character, Robot *robot);
    void use(Character *character, Robot *robot);
};

class FirstAid : public BaseItem {
public:
    FirstAid();
    bool canUse(Character *character, Robot *robot);
    void use(Character *character, Robot *robot);
};

class BaseBag {
protected:
    int capacity;
    int count;
    BaseItem* first_item;

public:
    BaseBag(int capacity);
    ~BaseBag();
    bool insert(BaseItem* in_item);
    BaseItem* get();
    BaseItem* get(ItemType type);
    BaseItem* getFirst();
    string str() const;
    int size() const;
    int getCount() const;
};

class SherlockBag : public BaseBag {
public:
    SherlockBag(Sherlock* sherlock);
};

class WatsonBag : public BaseBag {
public:
    WatsonBag(Watson* watson);
};

class ArrayMovingObject {
public:
    MovingObject** arr_mv_objs;

    int count;
    int capacity;
    ArrayMovingObject(int capacity);
    ~ArrayMovingObject();
    bool isFull() const;
    bool add(MovingObject* mv_obj);
    MovingObject* get(int index);
    int size() const;
    string str() const;
};

class Configuration {
public:
    int map_num_rows, map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    Position* arr_walls;
    int num_fake_walls;
    Position* arr_fake_walls;
    string sherlock_moving_rule;
    Position sherlock_init_pos;
    int sherlock_init_hp;
    int sherlock_init_exp;
    string watson_moving_rule;
    Position watson_init_pos;
    int watson_init_hp;
    int watson_init_exp;
    Position criminal_init_pos;
    int num_steps;



    Configuration(const string& filepath);
    ~Configuration();
    string str() const;
    friend class StudyPinkProgram;
};

class StudyPinkProgram {
public:
    Configuration* config;
    Sherlock* sherlock;
    Watson* watson;
    Criminal* criminal;
    Map* map;
    ArrayMovingObject* arr_mv_objs;
    
    bool stop;



    StudyPinkProgram(const string& config_file_path);
    bool isStop() const;
    void printResult() const;
    void printStep(int si) const;
    void createRobot();
    bool isMeet(MovingObject *mv_obj1, MovingObject *mv_obj2);
    void exchangeItem();

    void runInStep();

    void run(bool verbose);
    ~StudyPinkProgram();
};





////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
