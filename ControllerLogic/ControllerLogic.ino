#include <Stepper.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>





// ==============================================
// ---------- Start Sensor Functions ------------
//===============================================
// == == the sensor_update() function is the main tool in this section
// == == The sensor_update() function 
// == Sensor References
int sensor_right = A0;
int sensor_cent = A1;
int sensor_left = A2;


// == sensor array designations
const int sensor_samp_size = 6;   // -- The number of samples to base distance calculation off of
const int sensor_array_size = sensor_samp_size + 1; // - the total size of the array, including average value at the end
// == == Extra sensor array columns
const int sensor_adj_loc = sensor_array_size - 1; // - the index of the adjusted value in the array
    // -- adjusted value is the value remaining after the noise (errant highs/lows) has been cut out
//const int sensor_prnt_loc = sensor_array_size - 1;

long sensor_right_dists[sensor_array_size];
long sensor_cent_dists[sensor_array_size];
long sensor_left_dists[sensor_array_size];

void array_queue(long in_array[sensor_array_size], long value){
  // == Allows the provided array to behave like a queue, (FIFO)
  for(int x = sensor_samp_size - 1; x >= 1; x--){
    in_array[x] = in_array[x - 1];
  }
  in_array[0] = value;
}

void bubble_sort(long in_array[sensor_samp_size]){
  // -- -- -- -- 1) Create a flag for whether a swap has been made
  // -- -- -- -- 2) itterate through the array elements
  // -- -- -- -- -- 2a) if the next element of the array is less than the current element, swap
  // -- -- -- -- -- 2b) set flag to show a swap was made
  // -- -- -- -- -- 2c) return to step 2a, untill the last element has been reached
  // -- -- -- -- 3) if the swap flag has been triggered, reset flag and restart at step 2
  // -- -- -- -- -- if the flag has not been triggered, elements are sorted
  bool swap = 0;
  for (int sort_ind = 0; sort_ind < (sensor_samp_size - 1); sort_ind++){
    if(in_array[sort_ind] > in_array[sort_ind + 1]){
      long buffer = in_array[sort_ind + 1];
      in_array[sort_ind + 1] = in_array[sort_ind];
      in_array[sort_ind] = buffer;
      swap = 1;
    }
}
  if(swap == 1){
    bubble_sort(in_array);
  }
}

void array_median(long in_array[sensor_array_size]){
  // == updates the last element of the provided array (in_array) to be the median of 
  // == == the preceeding elements, via bubble sort (bubble_sort())

  // == 1) sort the sort array
  long sort_array[sensor_samp_size];              // -- create the sort array
  for(int x = 0; x < sensor_samp_size; x++){      // -- load the sort array
    sort_array[x] = in_array[x];}                 // -- load 1:1
  bubble_sort(sort_array);

  // == 2) Determine the Median value
  long median;
  if(sensor_samp_size % 2 == 0){
    // -- -- -- if sample size is even, take average of middle two values
    median = (sort_array[sensor_array_size / 2] + sort_array[(sensor_array_size / 2) - 1]) / 2;
  }
  if(sensor_samp_size % 2 == 1){
    // -- -- -- if the sample size is odd, take middle value
    median = sort_array[(sensor_array_size - 1) / 2];
  }

  // == 3) Update sample array with median value
  in_array[sensor_adj_loc] = median;
}

long range_find(int sensor_pin){
  // == send a square wave to a sensor (sensor_pin), return the resulting distance in cm (cm_dist)
  pinMode(sensor_pin, OUTPUT);
  digitalWrite(sensor_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor_pin, LOW);

  // == get the response time from the sensor, convert to cm
  pinMode(sensor_pin, INPUT);
  long raw_dist = pulseIn(sensor_pin, HIGH);
  long cm_dist = raw_dist / (29 * 2);
  if (cm_dist > 30){
    cm_dist = 30;
  }
  return cm_dist;
}

void print_array(long in_array[sensor_array_size], String pref){
  // == used for debugging purposes, prints out each value of a given array (in_array)
  // == == with a provided preface (perf)
  Serial.println(" ");
  Serial.print(pref);
  Serial.print(": ");
  for (int x = 0; x < sensor_array_size; x++){
    if(x != 0){
      Serial.print(", ");
    }
    Serial.print(in_array[x]);
  }
}


void sensor_update(int sensor, long in_array){
  // == read the distance of a given sensor (sensor, range_find()), and add the value
  // == == to the provided array (in_array), via queue operation (array_queue()) to retain chronology.
  // == == update current median value of the array (array_median())
  long dist_read = range_find(sensor);
  array_queue(in_array, dist_read);
  array_median(in_array);
}

void sensor_check(){
  // == Calls sensor_update() function on all sensors, which adds a new value
  // == == sensor sample to the sensor array, and updates the median value
  int sensor_ping_delay = 50;
  sensor_update(sensor_right, sensor_right_dists);
  delay(sensor_ping_delay);
  sensor_update(sensor_cent, sensor_cent_dists);
  delay(sensor_ping_delay);
  sensor_update(sensor_left, sensor_left_dists);
  delay(sensor_ping_delay);

  print_array(sensor_right_dists, "Right: ");
  print_array(sensor_cent_dists, "Center: ");
  print_array(sensor_left_dists, "Left: ");
}

// ==============================================
// XXXXXXXXXX End Sensor Functions XXXXXXXXXXXXXX
//===============================================

// ==============================================
// ----------- Start Servo Functions ------------
//===============================================

// == Servo References
int pwm_delay = 50;
int servo_pin = 9;
Servo turn_servo;

int turn_deg_center = 55;
int turn_deg_right = 30;
//** updated again from 35 to 30
//** updated from 40 to 35 (initial version)
int turn_deg_left = 90;
//** updated again from 85 to 90
//** updated from 80 to 85(initial version)
int turn_previous = turn_deg_right;    // for reverse turning
int turn_direction = 0; // indicates the current direction of the stearing motor
  // -1 = left, 0 = center, 1 = right

void turn_stearing(int dir){
  // -- Set motor direction
  for (int inc = 0; inc <= 10; inc++){
    turn_servo.write(dir);
  }
  // -- Update prev_tur flag
  if (dir == turn_deg_left){
    turn_previous = -1;
  }
  if (dir == turn_deg_right){
    turn_previous = 1;
  }
}

// ==============================================
// XXXXXXXXXXXX End Servo Functions XXXXXXXXXXXXX
//===============================================

// ==============================================
// --------- Start Stepper Functions ------------
//===============================================
// == Stepper References
int total_steps = 200;
int step_inc = 200;
int step_speed = 60;
int prev_drive_dir = 0;        
Stepper stepper(total_steps, 5, 3, 4, 2); // middle pins need to be flipped to function correctly
int drive_forward = -1;
int drive_reverse = 1;
int rev_adj = 6; // the reverse operations require more drive time, to clear obsticals
int drive_direction = drive_forward; // indicates the current direction of motion
  // 1 = Backwards, 0 = stopped, -1 = forward

void stepper_drive(int dir){
  stepper.step(step_inc * dir);
}
// ==============================================
// XXXXXXXXXXX End Stepper Functions XXXXXXXXXXXX
//===============================================

// ==============================================
// ----------- Start LCD Functions --------------
//===============================================
//char top_line[16] = "L=X | C=X | R=X ";    // - spots at 2, 8, and 14

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
// -- http://www.handsontec.com/dataspecs/module/I2C_1602_LCD.pdf
// -- -- Create the lcd i2c reference, useing
// -- -- adr = burnt in address
// -- -- en, rw, rs = just as they are on the board
// -- -- d1, d4, d5, d6, d7 = the D0-D5 pins



void update_screen(char new_l, char new_c, char new_r, char new_d, char new_s){
  char top_line[16] = "L=X | C=X | R=X ";    // - spots at 2, 8, and 14
  char bottom_line[16] = "   X   |    X   ";   // - open spots at 4 and 13
  // -- clear the screen
  lcd.clear();

  // -- print the top row
  top_line[2] = new_l;
  top_line[8] = new_c;
  top_line[14] = new_r;
  lcd.setCursor(0, 0);
  lcd.print(top_line);

   // -- print bottom row
  bottom_line[3] = new_d;
  bottom_line[12] = new_s;
  lcd.setCursor(0, 1);
  lcd.print(bottom_line);
}


// ==============================================
// ------------ End LCD Functions ---------------
//===============================================


// ==============================================
// ---------- Begin Logic Functions -------------
//===============================================

void sense_logic(){
  int close_flag_dist = 15;
  //** Updated from 10 (Initial version)
  int flag_count = 0;
  char l_char = 'N';
  char c_char = 'N';
  char r_char = 'N';
  char d_char = 'F';
  char s_char = 'C';

  // for each sensor who's read is LT-EQ to teh flag distance, adjust the flag_count
  // -- flag count is not a linear count, but an encoding
  // -- left = 1, center = 2, right = 4
  if (sensor_left_dists[sensor_adj_loc] <= close_flag_dist) {
    flag_count = flag_count + 1;
    l_char = '!';
  }
  if (sensor_cent_dists[sensor_adj_loc] <= close_flag_dist) {
    flag_count = flag_count + 2;
    c_char = '!';
  }
  if (sensor_right_dists[sensor_adj_loc] <= close_flag_dist) {
    flag_count = flag_count + 4;
    r_char = '!';
  }
  Serial.println(" ");
  Serial.println(flag_count);
  switch(flag_count){
    case 0:
      // no triggers
      // -- -- Set comm values
      // - turn Center
      s_char = 'C';
      // - drive forward
      d_char = 'F';

      // -- -- orders
      turn_stearing(turn_deg_center);
      stepper_drive(drive_forward);

      break;

    case 1:
      // left triggered
      // -- -- Set comm values
      // -- turn right
      s_char = 'R';
      // -- drive forward
      d_char = 'F';

      // -- -- orders
      turn_stearing(turn_deg_right);
      turn_previous = turn_deg_right;
      stepper_drive(drive_forward);
      break;

    case 2:
      // center triggered
      // -- -- Set comm values
      // -- turn same direction as previous turn
      int rev_dir;
      if (turn_previous == turn_deg_left) {
        // turn left
        s_char = 'L';
        rev_dir = turn_deg_left;
      }
      if (turn_previous == turn_deg_right) {
        // turn right
        s_char = 'R';
        rev_dir = turn_deg_right;
      }
      // -- drive reverse
      d_char = 'R';

      // -- -- orders
      turn_stearing(rev_dir);
      stepper_drive(drive_reverse * rev_adj);
      break;

    case 3:
      // left and center triggered
      // -- -- Set comm values
      // -- turn left
      s_char = 'L';
      // -- drive reverse
      d_char = 'R';

      // -- -- orders
      turn_stearing(turn_deg_left);
      stepper_drive(drive_reverse * rev_adj);
      break;

    case 4:
      // right triggered
      // -- -- Set comm values
      // -- turn left
      s_char = 'L';
      // -- drive forward
      d_char = 'F';

      // -- -- orders
      turn_stearing(turn_deg_left);
      stepper_drive(drive_forward);
      break;

    case 5:
      // left and right triggered
      // -- -- Set comm values
      // -- turn center
      s_char = 'C';
      // -- drive forward
      d_char = 'F';

      // -- -- orders
      turn_stearing(turn_deg_center);
      stepper_drive(drive_forward);
      break;

    case 6:
      // center and right triggered
      // -- -- Set comm values
      // -- turn right
      s_char = 'R';
      // -- drive reverse
      d_char = 'R';

      // -- -- orders
      turn_stearing(turn_deg_center);
      stepper_drive(drive_forward);
      break;

    case 7:
      char s_char_1;
      char s_char_2;
      int tpt_for;
      int tpt_rev;
      if (turn_previous == turn_deg_right) {
        tpt_rev = turn_deg_right;
        tpt_for = turn_deg_left;
        s_char_1 = 'R';
        s_char_2 = 'L';
      }
      if (turn_previous == turn_deg_left) {
        tpt_rev = turn_deg_left;
        tpt_for = turn_deg_right;
        s_char_1 = 'L';
        s_char_2 = 'R';
      }

      s_char = s_char_1;
      d_char = 'R';
      turn_stearing(tpt_rev);
      stepper_drive(drive_reverse * rev_adj);

      break;
    default:
      // should not happen
      break;
  }
  update_screen(l_char, c_char, r_char, d_char, s_char);
}

// ==============================================
// ----------- End Logic Functions --------------
//===============================================


void setup() {
  Serial.begin(9600);
  stepper.setSpeed(step_speed);
  turn_servo.attach(servo_pin);
  lcd.begin(16, 2);
  lcd.backlight();
}

void logic_loop(){
  // -- populate sensor arrays with current information
  for (int pop_x = 0; pop_x < sensor_samp_size; pop_x++){
    sensor_check();
  }

  // -- run loop for a certain number of times
  //for (int loop_x = 0; loop_x < 5; loop_x++){
  while (true){
    for (int check_x = 0; check_x < ((sensor_samp_size / 2) + 1); check_x++){
      sensor_check();
    }
    sense_logic();
  }
  
}

void loop() {
  int rout_delay = 1000;
  Serial.println("----------Process Starting---------");
  logic_loop();
  //sensor_debug();
  //motor_debug();
  // -- 1) Check surroundings
  //sensor_check();

  // -- 2) Adjust servo accordingly

  // -- 3) Engage Stepper forward/backward

  // -- update user
  //update_screen();
  delay(rout_delay);
}