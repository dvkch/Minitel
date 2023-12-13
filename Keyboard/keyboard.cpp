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

int rows[ROWS] = {14, 15, 18, 23, 24, 25, 8, 7, 11};
int cols[COLS] = {9, 10, 22, 27, 17, 28, 29, 30, 31};

typedef struct {
    const char *id;
    const char *name;
    std::vector<int> codes;
    std::vector<int> shift_codes;
    bool is_shift;
} Key;

Key keyboard[ROWS * COLS] = {
    // Row 0
    { "x0y0", "Left Shift",     {KEY_LEFTSHIFT},            {0},                                true },
    { "x0y1", "W",              {KEY_Z},                    {0},                                false },
    { "x0y2", "B",              {KEY_B},                    {0},                                false },
    { "x0y3", "N",              {KEY_N},                    {0},                                false },
    { "x0y4", "Right Shift",    {KEY_RIGHTSHIFT},           {0},                                true },
    { "x0y5", "V",              {KEY_V},                    {0},                                false },
    { "x0y6", "C",              {KEY_C},                    {0},                                false },
    { "x0y7", "X",              {KEY_X},                    {0},                                false },
    { "x0y8", "<x0y8>",         {0},                        {0},                                false },

    // Row 1
    { "x1y0", "Q",              {KEY_A},                    {0},                                false },
    { "x1y1", "D",              {KEY_D},                    {0},                                false },
    { "x1y2", "G",              {KEY_G},                    {0},                                false },
    { "x1y3", "J",              {KEY_J},                    {0},                                false },
    { "x1y4", "L",              {KEY_L},                    {0},                                false },
    { "x1y5", "1/`",            {KEY_KP1},                  {KEY_RIGHTALT, KEY_7},              false },
    { "x1y6", "2/(",            {KEY_KP2},                  {KEY_5},                            false },
    { "x1y7", "3/)",            {KEY_KP3},                  {KEY_MINUS},                        false },
    { "x1y8", "<x1y8>",         {0},                        {0},                                false },

    // Row 2
    { "x2y0", "Control",        {KEY_LEFTCTRL},             {0},                                false },
    { "x2y1", "S",              {KEY_S},                    {0},                                false },
    { "x2y2", "F",              {KEY_F},                    {0},                                false },
    { "x2y3", "H",              {KEY_H},                    {0},                                false },
    { "x2y4", "K",              {KEY_K},                    {0},                                false },
    { "x2y5", "M",              {KEY_SEMICOLON},            {0},                                false },
    { "x2y6", "P",              {KEY_P},                    {0},                                false },
    { "x2y7", "O",              {KEY_O},                    {0},                                false },
    { "x2y8", "<x2y8>",         {0},                        {0},                                false },

    // Row 3
    { "x3y0", "A",              {KEY_Q},                    {0},                                false },
    { "x3y1", "Z",              {KEY_W},                    {0},                                false },
    { "x3y2", "E",              {KEY_E},                    {0},                                false },
    { "x3y3", "R",              {KEY_R},                    {0},                                false },
    { "x3y4", "T",              {KEY_T},                    {0},                                false },
    { "x3y5", "Y",              {KEY_Y},                    {0},                                false },
    { "x3y6", "U",              {KEY_U},                    {0},                                false },
    { "x3y7", "I",              {KEY_I},                    {0},                                false },
    { "x3y8", "<x3y8>",         {0},                        {0},                                false },

    // Row 4
    { "x4y0", "Escape",         {KEY_ESC},                  {0},                                false },
    { "x4y1", ",/<",            {KEY_M},                    {KEY_102ND},                        false },
    { "x4y2", "./>",            {KEY_LEFTSHIFT, KEY_COMMA}, {KEY_LEFTSHIFT, KEY_102ND},         false },
    { "x4y3", "'/@",            {KEY_4},                    {KEY_RIGHTALT, KEY_0},              false },
    { "x4y4", ";/+",            {KEY_COMMA},                {KEY_LEFTSHIFT, KEY_EQUAL},         false },
    { "x4y5", "-/=",            {KEY_6},                    {KEY_EQUAL},                        false },
    { "x4y6", ":/*",            {KEY_DOT},                  {KEY_BACKSLASH},                    false },
    { "x4y7", "?//",            {KEY_LEFTSHIFT, KEY_M},     {KEY_LEFTSHIFT, KEY_DOT},           false },
    { "x4y8", "<x4y8>",         {0},                        {0},                                false },

    // Row 5
    { "x5y0", "Connexion",      {KEY_F1},                   {0},                                false },
    { "x5y1", "Guide/¨",        {KEY_F3},                   {KEY_LEFTSHIFT, KEY_LEFTBRACE},     false },
    { "x5y2", "Correction/§",   {KEY_BACKSPACE},            {KEY_LEFTSHIFT, KEY_SLASH},         false },
    { "x5y3", "Suite",          {KEY_FORWARD},              {0},                                false },
    { "x5y4", "Envoi/}",        {KEY_LEFTMETA},             {KEY_RIGHTALT, KEY_EQUAL},          false },
    { "x5y5", "4/$",            {KEY_KP4},                  {KEY_RIGHTBRACE},                   false },
    { "x5y6", "5/%",            {KEY_KP5},                  {KEY_LEFTSHIFT, KEY_APOSTROPHE},    false },
    { "x5y7", "6/&",            {KEY_KP6},                  {KEY_1},                            false },
    { "x5y8", "<x5y8>",         {0},                        {0},                                false },

    // Row 6
    { "x6y0", "Fonction",       {KEY_LEFTALT},              {0},                                false },
    { "x6y1", "Sommaire/^",     {KEY_F2},                   {KEY_LEFTBRACE},                    false },
    { "x6y2", "Annulation/\\",  {KEY_F4},                   {KEY_RIGHTALT, KEY_8},              false },
    { "x6y3", "Retour",         {KEY_BACK},                 {0},                                false },
    { "x6y4", "Répétition/{",   {KEY_F5},                   {KEY_RIGHTALT, KEY_4},              false },
    { "x6y5", "7/!",            {KEY_KP7},                  {KEY_SLASH},                        false },
    { "x6y6", "8/\"",           {KEY_KP8},                  {KEY_3},                            false },
    { "x6y7", "9/#",            {KEY_KP9},                  {KEY_RIGHTALT, KEY_3},              false },
    { "x6y8", "<x6y8>",         {0},                        {0},                                false },

    // Row 7
    { "x7y0", "Up",             {KEY_UP},                   {0},                                false },
    { "x7y1", "Down",           {KEY_DOWN},                 {0},                                false },
    { "x7y2", "Left",           {KEY_LEFT},                 {0},                                false },
    { "x7y3", "Right",          {KEY_RIGHT},                {0},                                false },
    { "x7y4", "Return",         {KEY_ENTER},                {0},                                false },
    { "x7y5", "NumLock/[",      {KEY_NUMLOCK},              {KEY_RIGHTALT, KEY_5},              false },
    { "x7y6", "0/|",            {KEY_KP0},                  {KEY_RIGHTALT, KEY_6},              false },
    { "x7y7", "Compose/]",      {KEY_RIGHTCTRL},            {KEY_RIGHTALT, KEY_MINUS},          false },
    { "x7y8", "Space/Tab",      {KEY_SPACE},                {KEY_TAB},                          false },

    // Row 8
    { "x8y0", "<x8y0>",         {0},                        {0},                                false },
    { "x8y1", "<x8y1>",         {0},                        {0},                                false },
    { "x8y2", "<x8y2>",         {0},                        {0},                                false },
    { "x8y3", "<x8y3>",         {0},                        {0},                                false },
    { "x8y4", "<x8y4>",         {0},                        {0},                                false },
    { "x8y5", "<x8y5>",         {0},                        {0},                                false },
    { "x8y6", "<x8y6>",         {0},                        {0},                                false },
    { "x8y7", "<x8y7>",         {0},                        {0},                                false },
    { "x8y8", "<x8y8>",         {0},                        {0},                                false },
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
        }

        void run() {
            if (rpi_gpio_init() < 0) {
                perror("Failed to initialize RPi GPIO");
                return;
            }
            for (int i = 0; i < ROWS; i++) {
                rpi_gpio_setup(rows[i], INPUT, PUD_OFF);
            }
            for (int i = 0; i < COLS; i++) {
                rpi_gpio_setup(cols[i], INPUT, PUD_UP);
            }

            while (true) {
                usleep(1000000 / 60); // Sleep for 1/60 seconds
                bool syn = false;

                for (int i = 0; i < ROWS; ++i) {
                    rpi_gpio_output(rows[i], 0);
                    rpi_gpio_set_fn(rows[i], OUTPUT);
                    usleep(3);
                    uint32_t all = rpi_gpio_input_all();
                    rpi_gpio_set_fn(rows[i], INPUT);

                    for (int j = 0; j < COLS; ++j) {
                        int keycode = i * COLS + j;
                        int newval = !(all & (1 << cols[j]));

                        if (newval && pressed.find(keycode) == pressed.end()) {
                            this->pressed.insert(keycode);
                            bool is_shift = keyboard[keycode].is_shift;
                            if (is_shift) {
                                for (int code : keyboard[keycode].codes) {
                                    this->shifts.insert(code);
                                }
                            }

                            if (this->printCoords) {
                                printf("%s down\n", keyboard[keycode].id);
                            }
                            if (this->printNames) {
                                printf("%s down\n", keyboard[keycode].name);
                            }
                            if (this->input) {
                                this->sendKey(keyboard[keycode], true);
                                syn = true;
                            }

                        } 
                        else if (!newval && pressed.find(keycode) != pressed.end()) {
                            this->pressed.erase(keycode);
                            bool is_shift = keyboard[keycode].is_shift;
                            if (is_shift) {
                                for (int code : keyboard[keycode].codes) {
                                    this->shifts.erase(code);
                                }
                            }

                            if (this->printCoords) {
                                printf("%s up\n", keyboard[keycode].id);
                            }
                            if (this->printNames) {
                                printf("%s up\n", keyboard[keycode].name);
                            }
                            if (this->input) {
                                this->sendKey(keyboard[keycode], false);
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

        void sendKey(const Key& key_config, bool pressed) {
            std::vector<int> keys = key_config.codes;
            bool using_shift_codes = false;

            if (!shifts.empty() && !key_config.shift_codes.empty()) {
                keys = key_config.shift_codes;
                using_shift_codes = true;
            }

            if (!pressed) {
                std::reverse(keys.begin(), keys.end());
            }

            int value = pressed ? 1 : 0;

            if (using_shift_codes) {
                int revert_shift_value = pressed ? 0 : 1;
                for (int shift : shifts) {
                    this->input->send_key(shift, revert_shift_value);
                }
            }

            for (int key : keys) {
                this->input->send_key(key, value);
            }

            if (using_shift_codes) {
                int revert_shift_value = pressed ? 1 : 0;
                for (int shift : shifts) {
                    this->input->send_key(shift, revert_shift_value);
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

