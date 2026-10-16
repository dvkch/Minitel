#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <functional>
#include <vector>
#include <set>
#include <algorithm>
#include <unistd.h>
#include <fcntl.h>

#include <linux/uinput.h>
#include "rpi_gpio.h"

#define ROWS 9
#define COLS 9

int rows[ROWS] = {14, 15, 18, 23, 24, 25, 8, 7, 1};
int cols[COLS] = {6, 5, 0, 11, 9, 10, 22, 27, 17};

typedef struct {
    const char *id;
    const char *name;
    std::vector<int> codes;
    std::vector<int> shift_codes;
    std::vector<int> fn_codes;
    bool is_shift;
    bool is_fn;
} Key;

Key keyboard[ROWS * COLS] = {
    // Row 0
    { "x0y0", "Shift",          {KEY_LEFTSHIFT},            {},                                 {},                         true,  false },
    { "x0y1", "W",              {KEY_Z},                    {},                                 {},                         false, false },
    { "x0y2", "B",              {KEY_B},                    {},                                 {},                         false, false },
    { "x0y3", "N",              {KEY_N},                    {},                                 {},                         false, false },
    { "x0y4", "Alt",            {KEY_LEFTALT},              {},                                 {},                         false, false }, // used as Alt
    { "x0y5", "V",              {KEY_V},                    {},                                 {},                         false, false },
    { "x0y6", "C",              {KEY_C},                    {},                                 {},                         false, false },
    { "x0y7", "X",              {KEY_X},                    {},                                 {},                         false, false },
    { "x0y8", "<x0y8>",         {},                         {},                                 {},                         false, false },

    // Row 1
    { "x1y0", "Q",              {KEY_A},                    {},                                 {},                         false, false },
    { "x1y1", "D",              {KEY_D},                    {},                                 {},                         false, false },
    { "x1y2", "G",              {KEY_G},                    {},                                 {},                         false, false },
    { "x1y3", "J",              {KEY_J},                    {},                                 {},                         false, false },
    { "x1y4", "L",              {KEY_L},                    {},                                 {},                         false, false },
    { "x1y5", "1",              {KEY_KP1},                  {KEY_RIGHTALT, KEY_7},              {KEY_F1},                   false, false },
    { "x1y6", "2",              {KEY_KP2},                  {KEY_5},                            {KEY_F2},                   false, false },
    { "x1y7", "3",              {KEY_KP3},                  {KEY_MINUS},                        {KEY_F3},                   false, false },
    { "x1y8", "<x1y8>",         {},                         {},                                 {},                         false, false },

    // Row 2
    { "x2y0", "Ctrl",           {KEY_LEFTCTRL},             {},                                 {},                         false, false },
    { "x2y1", "S",              {KEY_S},                    {},                                 {},                         false, false },
    { "x2y2", "F",              {KEY_F},                    {},                                 {},                         false, false },
    { "x2y3", "H",              {KEY_H},                    {},                                 {},                         false, false },
    { "x2y4", "K",              {KEY_K},                    {},                                 {},                         false, false },
    { "x2y5", "M",              {KEY_SEMICOLON},            {},                                 {},                         false, false },
    { "x2y6", "P",              {KEY_P},                    {},                                 {},                         false, false },
    { "x2y7", "O",              {KEY_O},                    {},                                 {},                         false, false },
    { "x2y8", "<x2y8>",         {},                         {},                                 {},                         false, false },

    // Row 3
    { "x3y0", "A",              {KEY_Q},                    {},                                 {},                         false, false },
    { "x3y1", "Z",              {KEY_W},                    {},                                 {},                         false, false },
    { "x3y2", "E",              {KEY_E},                    {},                                 {KEY_RIGHTALT, KEY_E},      false, false },
    { "x3y3", "R",              {KEY_R},                    {},                                 {},                         false, false },
    { "x3y4", "T",              {KEY_T},                    {},                                 {},                         false, false },
    { "x3y5", "Y",              {KEY_Y},                    {},                                 {},                         false, false },
    { "x3y6", "U",              {KEY_U},                    {},                                 {},                         false, false },
    { "x3y7", "I",              {KEY_I},                    {},                                 {},                         false, false },
    { "x3y8", "<x3y8>",         {},                         {},                                 {},                         false, false },

    // Row 4
    { "x4y0", "Esc",            {KEY_ESC},                  {},                                 {},                         false, false },
    { "x4y1", ",",              {KEY_M},                    {KEY_102ND},                        {},                         false, false },
    { "x4y2", ".",              {KEY_LEFTSHIFT, KEY_COMMA}, {KEY_LEFTSHIFT, KEY_102ND},         {},                         false, false },
    { "x4y3", "'",              {KEY_4},                    {KEY_RIGHTALT, KEY_0},              {},                         false, false },
    { "x4y4", ";",              {KEY_COMMA},                {KEY_LEFTSHIFT, KEY_EQUAL},         {},                         false, false },
    { "x4y5", "-",              {KEY_6},                    {KEY_EQUAL},                        {KEY_RIGHTALT, KEY_2},      false, false },
    { "x4y6", ":",              {KEY_DOT},                  {KEY_BACKSLASH},                    {},                         false, false },
    { "x4y7", "?",              {KEY_LEFTSHIFT, KEY_M},     {KEY_LEFTSHIFT, KEY_DOT},           {},                         false, false },
    { "x4y8", "<x4y8>",         {},                         {},                                 {},                         false, false },

    // Row 5
    { "x5y0", "Connexion",      {KEY_LEFTMETA},             {},                                 {KEY_POWER},                false, false }, // Power: shuts the Pi down, can't start it
    { "x5y1", "Guide",          {KEY_END},                  {KEY_LEFTSHIFT, KEY_LEFTBRACE},     {},                         false, false },
    { "x5y2", "Correction",     {KEY_BACKSPACE},            {KEY_LEFTSHIFT, KEY_SLASH},         {},                         false, false },
    { "x5y3", "Suite",          {KEY_FORWARD},              {KEY_RIGHTALT, KEY_BACKSLASH},      {KEY_VOLUMEUP},             false, false }, // dead grave
    { "x5y4", "Envoi",          {KEY_PAGEDOWN},             {KEY_RIGHTALT, KEY_EQUAL},          {},                         false, false },
    { "x5y5", "4",              {KEY_KP4},                  {KEY_RIGHTBRACE},                   {KEY_F4},                   false, false },
    { "x5y6", "5",              {KEY_KP5},                  {KEY_LEFTSHIFT, KEY_APOSTROPHE},    {KEY_F5},                   false, false },
    { "x5y7", "6",              {KEY_KP6},                  {KEY_1},                            {KEY_F6},                   false, false },
    { "x5y8", "<x5y8>",         {},                         {},                                 {},                         false, false },

    // Row 6
    { "x6y0", "Fnct",           {},                         {},                                 {},                         false, true  },
    { "x6y1", "Sommaire",       {KEY_HOME},                 {KEY_LEFTBRACE},                    {},                         false, false },
    { "x6y2", "Annulation",     {KEY_DELETE},               {KEY_RIGHTALT, KEY_8},              {},                         false, false },
    { "x6y3", "Retour",         {KEY_BACK},                 {KEY_RIGHTALT, KEY_M},              {KEY_VOLUMEDOWN},           false, false }, // dead acute
    { "x6y4", "Répétition",     {KEY_PAGEUP},               {KEY_RIGHTALT, KEY_4},              {},                         false, false },
    { "x6y5", "7",              {KEY_KP7},                  {KEY_SLASH},                        {KEY_F7},                   false, false },
    { "x6y6", "8",              {KEY_KP8},                  {KEY_3},                            {KEY_F8},                   false, false },
    { "x6y7", "9",              {KEY_KP9},                  {KEY_RIGHTALT, KEY_3},              {KEY_F9},                   false, false },
    { "x6y8", "<x6y8>",         {},                         {},                                 {},                         false, false },

    // Row 7
    { "x7y0", "↑",              {KEY_UP},                   {},                                 {},                         false, false },
    { "x7y1", "↓",              {KEY_DOWN},                 {},                                 {},                         false, false },
    { "x7y2", "←",              {KEY_LEFT},                 {},                                 {},                         false, false },
    { "x7y3", "→",              {KEY_RIGHT},                {},                                 {},                         false, false },
    { "x7y4", "Return",         {KEY_ENTER},                {},                                 {},                         false, false },
    { "x7y5", "*",              {KEY_NUMLOCK},              {KEY_RIGHTALT, KEY_5},              {},                         false, false },
    { "x7y6", "0",              {KEY_KP0},                  {KEY_RIGHTALT, KEY_6},              {},                         false, false },
    { "x7y7", "#",              {KEY_RIGHTCTRL},            {KEY_RIGHTALT, KEY_MINUS},          {},                         false, false }, // Compose key with the host's compose:rctrl option
    { "x7y8", "Espace",         {KEY_SPACE},                {KEY_TAB},                          {},                         false, false },

    // Row 8
    { "x8y0", "<x8y0>",         {},                         {},                                 {},                         false, false },
    { "x8y1", "<x8y1>",         {},                         {},                                 {},                         false, false },
    { "x8y2", "<x8y2>",         {},                         {},                                 {},                         false, false },
    { "x8y3", "<x8y3>",         {},                         {},                                 {},                         false, false },
    { "x8y4", "<x8y4>",         {},                         {},                                 {},                         false, false },
    { "x8y5", "<x8y5>",         {},                         {},                                 {},                         false, false },
    { "x8y6", "<x8y6>",         {},                         {},                                 {},                         false, false },
    { "x8y7", "<x8y7>",         {},                         {},                                 {},                         false, false },
    { "x8y8", "<x8y8>",         {},                         {},                                 {},                         false, false },
};

class Input {
    public:
        Input() {
            fd = Input::create();
        }

        void send_key(int code, int value) {
            struct input_event ev;
            gettimeofday(&ev.time, NULL);

            ev.type = EV_KEY;
            ev.code = code;
            ev.value = value;

            if (write(this->fd, &ev, sizeof(ev)) < 0) {
                fprintf(stderr, "Error writing key event to uinput device\n");
            }
        }

        void send_syn() {
            struct input_event ev;
            gettimeofday(&ev.time, NULL);

            ev.type = EV_SYN;
            ev.code = SYN_REPORT;
            ev.value = 0;

            if (write(this->fd, &ev, sizeof(ev)) < 0) {
                fprintf(stderr, "Error syncing key event to uinput device\n");
            }
        }

    private:
        int fd;

        static int create() {
            int fd = -1;
            struct uinput_user_dev uidev;

            printf("---- Initializing keyboard ----\n");

            while (fd == -1) {
                fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
                if (fd == -1) {
                    fprintf(stderr, "Error opening /dev/uinput\n");
                    return -1;
                }

                // Set up the uinput device
                ioctl(fd, UI_SET_EVBIT, EV_KEY);
                for (int k = 0; k < ROWS * COLS; k++) {
                    for (int key : keyboard[k].codes) {
                        ioctl(fd, UI_SET_KEYBIT, key);
                    }
                    for (int key : keyboard[k].shift_codes) {
                        ioctl(fd, UI_SET_KEYBIT, key);
                    }
                    for (int key : keyboard[k].fn_codes) {
                        ioctl(fd, UI_SET_KEYBIT, key);
                    }
                }

                memset(&uidev, 0, sizeof(uidev));
                snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "Minitel Keyboard");
                uidev.id.bustype = BUS_USB;
                uidev.id.vendor  = 0x1209;
                uidev.id.product = 0x7501;

                if (ioctl(fd, UI_DEV_SETUP, &uidev) < 0) {
                    printf("Error writing to uinput device\n");
                    close(fd);
                    return -1;
                }

                if (ioctl(fd, UI_DEV_CREATE) < 0) {
                    printf("Error creating uinput device\n");
                    close(fd);
                    return -1;
                }

                printf("Initialized uinput device\n");
            }

            printf("-------------------------------\n");
            return fd;
        }
};

class Keyboard {
    public:
        Keyboard(bool printCoords, bool printNames, Input *input) {
            this->printCoords = printCoords;
            this->printNames = printNames;
            this->input = input;
            this->fn_held = false;
        }

        void run() {
            if (rpi_gpio_init() < 0) {
                perror("Failed to initialize RPi GPIO");
                return;
            }
            for (int i = 0; i < ROWS; i++) {
                rpi_gpio_setup(rows[i], INPUT, PUD_UP);
            }
            for (int i = 0; i < COLS; i++) {
                rpi_gpio_setup(cols[i], INPUT, PUD_UP);
            }

            while (true) {
                usleep(1000000 / 60); // Sleep for 1/60 seconds
                bool syn = false;

                uint16_t matrix[ROWS];
                for (int i = 0; i < ROWS; ++i) {
                    rpi_gpio_output(rows[i], 0);
                    rpi_gpio_set_fn(rows[i], OUTPUT);
                    usleep(3);
                    uint32_t all = rpi_gpio_input_all();
                    // a released row floats low long enough to ghost on the next one, drive it high first
                    rpi_gpio_output(rows[i], 1);
                    rpi_gpio_set_fn(rows[i], INPUT);

                    matrix[i] = 0;
                    for (int j = 0; j < COLS; ++j) {
                        if (!(all & (1 << cols[j]))) {
                            matrix[i] |= 1 << j;
                        }
                    }
                }

                for (int i = 0; i < ROWS; ++i) {
                    for (int j = 0; j < COLS; ++j) {
                        int keycode = i * COLS + j;
                        bool newval = matrix[i] & (1 << j);

                        if (newval && pressed.find(keycode) == pressed.end() && !isAmbiguous(matrix, i, j)) {
                            this->pressed.insert(keycode);
                            if (keyboard[keycode].is_shift) {
                                for (int code : keyboard[keycode].codes) {
                                    this->shifts.insert(code);
                                }
                            }
                            if (keyboard[keycode].is_fn) {
                                this->fn_held = true;
                            }

                            if (this->printCoords) {
                                printf("%s down\n", keyboard[keycode].id);
                            }
                            if (this->printNames) {
                                printf("%s down\n", keyboard[keycode].name);
                            }
                            if (this->input) {
                                this->sendKey(keycode, true);
                                syn = true;
                            }
                        }
                        else if (!newval && pressed.find(keycode) != pressed.end()) {
                            this->pressed.erase(keycode);
                            if (keyboard[keycode].is_shift) {
                                for (int code : keyboard[keycode].codes) {
                                    this->shifts.erase(code);
                                }
                            }
                            if (keyboard[keycode].is_fn) {
                                this->fn_held = false;
                            }

                            if (this->printCoords) {
                                printf("%s up\n", keyboard[keycode].id);
                            }
                            if (this->printNames) {
                                printf("%s up\n", keyboard[keycode].name);
                            }
                            if (this->input) {
                                this->sendKey(keycode, false);
                                syn = true;
                            }
                        }
                    }
                }

                if (syn && this->input) {
                    this->input->send_syn();
                }
            }
        }

    private:
        bool printCoords;
        bool printNames;
        Input *input;
        std::set<int> pressed;
        std::set<int> shifts;
        std::set<int> shifted_keys;
        bool fn_held;
        std::set<int> fn_keys;

        // Without diodes, 3 keys held at the corners of a rectangle make the 4th one look pressed. This happens
        // when two rows share at least two columns: a new press in such a configuration is ignored.
        static bool isAmbiguous(const uint16_t matrix[ROWS], int row, int col) {
            for (int i = 0; i < ROWS; ++i) {
                if (i != row && (matrix[i] & (1 << col)) && (matrix[i] & matrix[row] & ~(1 << col))) {
                    return true;
                }
            }
            return false;
        }

        void sendKey(int keycode, bool pressed) {
            const Key& key_config = keyboard[keycode];

            // the variant is chosen on press and reused on release, even if Shift or Fnct changed meanwhile.
            // Fnct wins over Shift, and a key without Fnct variant is sent as if Fnct wasn't held
            if (pressed && fn_held && !key_config.fn_codes.empty()) {
                fn_keys.insert(keycode);
            }
            else if (pressed && !shifts.empty() && !key_config.shift_codes.empty()) {
                shifted_keys.insert(keycode);
            }
            bool using_fn_codes = fn_keys.count(keycode) > 0;
            bool using_shift_codes = shifted_keys.count(keycode) > 0;
            if (!pressed) {
                fn_keys.erase(keycode);
                shifted_keys.erase(keycode);
            }

            std::vector<int> keys = using_fn_codes ? key_config.fn_codes : using_shift_codes ? key_config.shift_codes : key_config.codes;
            if (!pressed) {
                std::reverse(keys.begin(), keys.end());
            }

            // shifted variants are typed without Shift, which is restored once the key is released
            if (using_shift_codes && pressed) {
                for (int shift : shifts) {
                    this->input->send_key(shift, 0);
                }
            }

            for (int key : keys) {
                this->input->send_key(key, pressed ? 1 : 0);
            }

            if (using_shift_codes && !pressed) {
                for (int shift : shifts) {
                    this->input->send_key(shift, 1);
                }
            }
        }
};

void help() {
    printf("Unknown args: call this script using:\n");
    printf("- keyboard coords    # print the pushed key coords\n");
    printf("- keyboard names     # print the pushed key names\n");
    printf("- keyboard keyboard  # run as a virtual input\n");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        help();
        return 1;
    }

    if (strcmp(argv[1], "coords") == 0) {
        printf("Starting in key coords mode\n");
        Keyboard *k = new Keyboard(true, false, NULL);
        k->run();
    }
    else if (strcmp(argv[1], "names") == 0) {
        printf("Starting in key names mode\n");
        Keyboard *k = new Keyboard(false, true, NULL);
        k->run();
    }
    else if (strcmp(argv[1], "keyboard") == 0) {
        printf("Starting in UInput mode\n");
        Input *input = (strcmp(argv[1], "keyboard") == 0) ? new Input() : NULL;
        Keyboard *k = new Keyboard(false, false, input);
        k->run();
    }
    else {
        help();
        exit(1);
    }

    return 0;
}
