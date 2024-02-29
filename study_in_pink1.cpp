#include "study_in_pink1.h"
#include "main.h"

bool readFile(
        const string & filename,
        int & HP1,
        int & HP2,
        int & EXP1,
        int & EXP2,
        int & M1,
        int & M2,
        int & E1,
        int & E2,
        int & E3
) {
    // This function is used to read the input file,
    // DO NOT MODIFY THIS FUNTION
    ifstream ifs(filename);
    if (ifs.is_open()) {
        ifs >> HP1 >> HP2
            >> EXP1 >> EXP2
            >> M1 >> M2
            >> E1 >> E2 >> E3;
        return true;
    }
    else {
        cerr << "The file is not found" << endl;
        return false;
    }
}

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////
enum data_in{
    HP, EXP, M, E
};

int get_rounded_up(float number) {
    if (number - int(number) < 1e-4) {
        return int(number);
    }
    return int(number) + 1;
}

int get_data_in_range(float data, int data_type) {
    data = get_rounded_up(data);
    if (data < 0) {
        return 0;
    }
    if (data_type == HP) {
        if (data > 666) {
            return 666;
        }
    } else if (data_type == EXP) {
        if (data > 600) {
            return 600;
        }
    } else if (data_type == M) {
        if (data > 3000) {
            return 3000;
        }
    }
    return data;
}

bool is_E_valid(int E) {
    if (E < 0 || E > 99) {
        return false;
    }
    return true;
}

int nearest_square(int num) {
  if (num < 0) {
    throw invalid_argument("Input number must be non-negative.");
  }

  int sqrt_num = floor(sqrt(num));
  int square_less = sqrt_num * sqrt_num;
  int square_more = (sqrt_num + 1) * (sqrt_num + 1);

  return (abs(num - square_less) < abs(num - square_more)) ? square_less : square_more;
}

bool check_money(float money, float pay, float & HP1, float & EXP1, bool is_odd) {
    if (((pay > 0.5 * money) && is_odd) || ((money < 1) && !is_odd)) {
        HP1 -= HP1 * 0.17;
        EXP1 += EXP1 * 0.17;
        HP1 = get_data_in_range(HP1, HP);
        EXP1 = get_data_in_range(EXP1, EXP);
        return false;
    }
    return true;
}

int just_one_digit(int num) {
    while(num / 10 > 0) {
        num = num / 10 + num % 10;
    }
    return num;
}

int find_max_point(int arr[10][10], int i, int j) {
    int max_point = -999999;
    for (int k = i, l = j; k < 10 && l < 10; k++, l++) {
        if (arr[k][l] > max_point) {
            max_point = arr[k][l];
        }
    }
    for (int k = i, l = j; k >= 0 && l >= 0; k--, l--) {
        if (arr[k][l] > max_point) {
            max_point = arr[k][l];
        }
    }
    for (int k = i, l = j; k < 10 && l >= 0; k++, l--) {
        if (arr[k][l] > max_point) {
            max_point = arr[k][l];
        }
    }
    for (int k = i, l = j; k >= 0 && l < 10; k--, l++) {
        if (arr[k][l] > max_point) {
            max_point = arr[k][l];
        }
    }

    return abs(max_point);
}


enum type_of_password{
    VALID, SHORTER, LONGER, HAS_SE, DUPLICATE, NOT_HAS_SPECIAL_CHAR, DEFAULT
};
int return_value(int type, int index = 0) {
    if (type == VALID) {
        return -10;
    } else if (type == SHORTER) {
        return -1;
    } else if (type == LONGER) {
        return -2;
    } else if (type == HAS_SE) {
        return -(300 + index);
    } else if (type == DUPLICATE) {
        return -(400 + index);
    } else if (type == NOT_HAS_SPECIAL_CHAR) {
        return -5;
    } else {
        return index;
    }
}

////////////////////////////////////////////////////////////////////////
// Task 1
int firstMeet(int & exp1, int & exp2, int e1) {
    if (!is_E_valid(e1)) {
        return -99;
    }
    float EXP1 = get_data_in_range(exp1, EXP); // Sherlock
    float EXP2 = get_data_in_range(exp2, EXP); // Watson
    float E1 = e1;

    if (E1 <= 3) {
        if (E1 == 0) {
            EXP2 += 29;
        } else if (E1 == 1) {
            EXP2 += 45;
        } else if (E1 == 2) {
            EXP2 += 75;
        } else if (E1 == 3){
            EXP2 += 29 + 45 + 75;
        }
        EXP2 = get_data_in_range(EXP2, EXP);

        float D = E1 * 3 + EXP1 * 7;
        if (int(D) % 2 == 0) {
            EXP1 += D / 200;
        } else {
            EXP1 -= D / 100;
        }
        EXP1 = get_data_in_range(EXP1, EXP);

    } else if (4 <= E1 && E1 <= 99) {
        if (4 <= E1 && E1 <= 19) {
            EXP2 += E1 / 4 + 19;
        } else if (20 <= E1 && E1 <= 49) {
            EXP2 += E1 / 9 + 21;
            EXP2 = get_data_in_range(EXP2, EXP);
        } else if (50 <= E1 && E1 <= 65) {
            EXP2 += E1 / 16 + 17;
        } else if (66 <= E1 && E1 <= 79) {
            EXP2 += E1 / 4 + 19;
            EXP2 = get_data_in_range(EXP2, EXP);
            if (EXP2 > 200) {
                EXP2 += E1 / 9 + 21;
            }
        } else if (80 <= E1 && E1 <= 99) {
            EXP2 += E1 / 4 + 19;
            EXP2 = get_data_in_range(EXP2, EXP);
            EXP2 += E1 / 9 + 21;
            EXP2 = get_data_in_range(EXP2, EXP);
            if (EXP2 > 400) {
                EXP2 += E1 / 16 + 17;
                EXP2 = get_data_in_range(EXP2, EXP);
                EXP2 += EXP2 * 0.15;
            }
        }
        EXP2 = get_data_in_range(EXP2, EXP);

        EXP1 -= E1;
        EXP1 = get_data_in_range(EXP1, EXP);
    }

    exp1 = EXP1;
    exp2 = EXP2;

    return exp1 + exp2;
}

// Task 2
int traceLuggage(int & hp1, int & exp1, int & m1, int e2) {
    if (!is_E_valid(e2)) {
        return -99;
    }
    float HP1 = get_data_in_range(hp1, HP);
    float EXP1 = get_data_in_range(exp1, EXP);
    float M1 = get_data_in_range(m1, M);
    float E2 = e2;
    float nail_M1 = M1;

    // WAY 1
    float S = nearest_square(EXP1);
    float P1;
    if (EXP1 >= S) {
        P1 = 1;
    } else {
        P1 = (EXP1 / S + 80) / 123;
    }

    // WAY 2
    float total_paid = 0;
    bool is_odd = ((int(E2) % 2) != 0);

    do {
        if (!check_money(nail_M1, 1, HP1, EXP1, false)) {
            break;
        }
        if (HP1 < 200) {
            if (!check_money(nail_M1, total_paid, HP1, EXP1, is_odd)) {
                break;
            }
            HP1 += HP1 * 0.3;
            M1 -= 150;
            total_paid += 150;
        } else {
            if (!check_money(nail_M1, total_paid, HP1, EXP1, is_odd)) {
                break;
            }
            HP1 += HP1 * 0.1;
            M1 -= 70;
            total_paid += 70;
        }
        HP1 = get_data_in_range(HP1, HP);
        M1 = get_data_in_range(M1, M);
        if (!check_money(M1, 1, HP1, EXP1, is_odd)) {
            break;
        }


        if (EXP1 < 400) {
            if (!check_money(nail_M1, total_paid, HP1, EXP1, is_odd)) {
                break;
            }
            M1 -= 200;
            total_paid += 200;
        } else {
            if (!check_money(nail_M1, total_paid, HP1, EXP1, is_odd)) {
                break;
            }
            M1 -= 120;
            total_paid += 120;
        }
        EXP1 += EXP1 * 0.13;
        EXP1 = get_data_in_range(EXP1, EXP);
        M1 = get_data_in_range(M1, M);
        if (!check_money(M1, 1, HP1, EXP1, is_odd)) {
            break;
        }

        if (EXP1 < 300) {
            if (!check_money(nail_M1, total_paid, HP1, EXP1, is_odd)) {
                break;
            }
            M1 -= 100;
            total_paid += 100;
        } else {
            if (!check_money(nail_M1, total_paid, HP1, EXP1, is_odd)) {
                break;
            }
            M1 -= 120;
            total_paid += 120;
        }
        EXP1 -= EXP1 * 0.1;
        EXP1 = get_data_in_range(EXP1, EXP);
        M1 = get_data_in_range(M1, M);
        if (!is_odd) {
            check_money(0, 1, HP1, EXP1, is_odd);
        }
    } while (is_odd);

    S = nearest_square(EXP1);
    float P2;
    if (EXP1 >= S) {
        P2 = 1;
    } else {
        P2 = (EXP1 / S + 80) / 123;
    }


    // WAY 3
    int P3_arr[10] = {32, 47, 28, 79, 100, 50, 22, 83, 64, 11};
    int i = 0;
    if (E2 < 10) {
        i = E2;
    } else {
        i = int((E2 / 10) + (int(E2) % 10)) % 10;
    }

    float P3 = P3_arr[i] / 100.0;

    if (abs(P1 + P2 + P3 - 3) < 1e-4) {
        EXP1 -= EXP1 * 0.25;
    } else {
        float P = (P1 + P2 + P3) / 3;
        if (P < 0.5) {
            HP1 -= HP1 * 0.15;
            EXP1 += EXP1 * 0.15;
        } else {
            HP1 -= HP1 * 0.1;
            EXP1 += EXP1 * 0.2;
        }
    }
    EXP1 = get_data_in_range(EXP1, EXP);
    HP1 = get_data_in_range(HP1, HP);

    hp1 = HP1;
    exp1 = EXP1;
    m1 = M1;

    return hp1 + exp1 + m1;
}

// Task 3
int chaseTaxi(int & HP1, int & EXP1, int & HP2, int & EXP2, int E3) {
    if (!is_E_valid(E3)) {
        return -99;
    }
    float hp1 = get_data_in_range(HP1, HP);
    float exp1 = get_data_in_range(EXP1, EXP);
    float hp2 = get_data_in_range(HP2, HP);
    float exp2 = get_data_in_range(EXP2, EXP);
    float e3 = E3;

    int arr[10][10];
    int meeting_location_i = 0;
    int meeting_location_j = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            arr[i][j] = (e3 * j + i * 2) * (i - j);
            if (arr[i][j] > (e3 * 2)) {
                meeting_location_i++;
            }
            if (arr[i][j] < (e3 * -1)) {
                meeting_location_j++;
            }
        }
    }
    meeting_location_i = just_one_digit(meeting_location_i);
    meeting_location_j = just_one_digit(meeting_location_j);
    int taxi_point = arr[meeting_location_i][meeting_location_j];
    int chaser_point = find_max_point(arr, meeting_location_i, meeting_location_j);
    if (abs(taxi_point) <= chaser_point) {
        hp1 += hp1 * 0.1;
        hp2 += hp2 * 0.1;
        exp1 += exp1 * 0.12;
        exp2 += exp2 * 0.12;
    } else {
        hp1 -= hp1 * 0.1;
        hp2 -= hp2 * 0.1;
        exp1 -= exp1 * 0.12;
        exp2 -= exp2 * 0.12;
    }

    HP1 = get_data_in_range(hp1, HP);
    EXP1 = get_data_in_range(exp1, EXP);
    HP2 = get_data_in_range(hp2, HP);
    EXP2 = get_data_in_range(exp2, EXP);

    if (abs(taxi_point) > chaser_point) {
        return taxi_point;
    }
    return chaser_point;
}

// Task 4
int checkPassword(const char * s, const char * email) {
    // Get name before @
    string email_name = "";
    for (int i = 0; i < strlen(email); i++) {
        if (email[i] == '@') {
            break;
        }
        email_name += email[i];
    }

    // Convert char to string
    string password = s;

    //VALID, SHORTER, LONGER, HAS_SE, DUPLICATE, NOT_HAS_SPECIAL_CHAR, DEFAULT
    char special_char[] = "@#%$!";
    int special_char_count = 0;

    // Check length
    if (password.length() < 8) {
        return return_value(SHORTER);
    } else if (password.length() > 20) {
        return return_value(LONGER);
    }

    // Check if has email name and return first index of email name
    if (password.find(email_name) != string::npos) {
        return return_value(HAS_SE, password.find(email_name));
    }

    // Check if has duplicate
    for (int i = 0; i < password.length() - 2; i++) {
        if ((password[i] == password[i + 1]) && (password[i] == password[i + 2])) {
            return return_value(DUPLICATE, i);
        }
    }

    // Check if does not have special char
    for (int i = 0; i < password.length(); i++) {
        for (int j = 0; j < strlen(special_char); j++) {
            if (password[i] == special_char[j]) {
                special_char_count++;
                break;
            }
        }
    }
    if (special_char_count == 0) {
        return return_value(NOT_HAS_SPECIAL_CHAR);
    }

    // Check if not is word or number or special char and return first invalid index
    for (int i = 0; i < strlen(s); i++) {
        if (!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= '0' && s[i] <= '9') || (s[i] == '@') || (s[i] == '#') || (s[i] == '%') || (s[i] == '$') || (s[i] == '!'))) {
            return return_value(DEFAULT, i);
        }
    }

    // Return valid
    return return_value(VALID);
}

// Task 5
int findCorrectPassword(const char * arr_pwds[], int num_pwds) {
    string* arr_pwds_string = new string[num_pwds];
    for (int i = 0; i < num_pwds; i++) {
        arr_pwds_string[i] = arr_pwds[i];
    }

    int max_appeared = 0;
    int max_appeared_index[30];
    int num_appeared[30];
    for (int i = 0; i < 30; i++) {
        max_appeared_index[i] = -1;
    }
    int index = 0;


    for (int i = 0; i < num_pwds; i++) {
        if (arr_pwds_string[i] == "") {
            continue;
        }

        int appeared = 1;
        for (int j = i + 1; j < num_pwds; j++) {
            if (arr_pwds_string[i] == arr_pwds_string[j]) {
                appeared++;
                arr_pwds_string[j] = "";
            }
        }
        arr_pwds_string[i] = "";

        if (appeared >= max_appeared) {
            max_appeared = appeared;
            max_appeared_index[index] = i;
            num_appeared[index] = max_appeared;
            index++;
        }
    }


    int longest_idx = -1;

    int i = 0;
    while (max_appeared_index[i] != -1) {
        if (num_appeared[i] == max_appeared) {
            if (longest_idx == -1) {
                longest_idx = i;
                i++;
                continue;
            }
            if (strlen(arr_pwds[max_appeared_index[i]]) > strlen(arr_pwds[max_appeared_index[longest_idx]])) {
                longest_idx = i;
            }
        }
        i++;
    }
    delete[] arr_pwds_string;
    return max_appeared_index[longest_idx];


}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////