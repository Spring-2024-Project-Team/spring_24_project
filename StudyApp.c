#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>

// Function to check if a key is pressed
int userInput(void) {
  struct timeval tv; // Declare a timeval structure
  fd_set fds;        // Declare a file descriptor set
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);              // Initialize the file descriptor set
  FD_SET(STDIN_FILENO, &fds); // check stdin for any input
  select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &fds);
}

void delay(int seconds) { sleep(seconds); }

void timer(char *topic) {
  int hours = 0, minutes = 0, seconds = 0;
  char input;

  printf("\nTimer started. Press 'q' to quit.\n");

  // Countdown loop
  while (1) {
    printf("\r%02d:%02d:%02d", hours, minutes, seconds);
    fflush(stdout);
    delay(1); // Delay for 1 second

    // Increment seconds, minutes, hours as needed
    seconds++;
    if (seconds == 60) {
      seconds = 0;
      minutes++;
      if (minutes == 60) {
        minutes = 0;
        hours++;
      }
    }

    // Check for user input to quit
    if (userInput()) {
      input = getchar();
      if (input == 'q' || input == 'Q') {
        printf("\nTimer stopped.\nYou studied %s for %d hours, %d minutes, "
               "and %d seconds\n",
               topic, hours, minutes, seconds - 1);
        break; // Exit the loop
      }
    }
  }
}

void alarmFunction() {
  time_t rawtime;
  struct tm *info;
  setenv("TZ", "GMT+4", 1); // changes time zone to eastern standard time
                            // (changed it to +4 due to daylight savings time)
  tzset();                  // sets time zone

  time(&rawtime);
  info = localtime(
      &rawtime); // takes local time from user and puts it into a struct
  printf("Current local time and date: %s\n", asctime(info));

  // take in user input for time
  int hour, minute, seconds;
  printf("What time would you like to stop studying? Enter in military time "
         "with hours, minute and seconds in spaces\n");
  scanf("%d %d %d", &hour, &minute, &seconds);

  // makes sure all of the inputs are valid
  if ((hour < 24 && hour > 0) && (minute > -1 && minute < 60) &&
      (seconds > -1 && seconds < 60)) {

    // retake the time so that the current time is accurate
    time(&rawtime);
    info = localtime(&rawtime);
    printf("\nAlarm started!\n");
    printf("\nUpdated time: %d:%d:%d\n", info->tm_hour, info->tm_min,
           info->tm_sec); // print current time

    // calculate the difference between end time and current time
    // make the calulcations into seconds
    int newTime = 0;
    newTime = 60 * 60 * (hour - info->tm_hour); // hrs to seconds
    newTime += 60 * (minute - info->tm_min);    // minutes to seconds
    newTime += (seconds - info->tm_sec);

    // use sleep function until that amount of time has passed
    sleep(newTime);
    printf(
        "Alarm done!\n"); // when that amount of time has passed, timer is done
  } else
    printf("You entered invalid time format\n");
}

void toDoList() {
  char toDoList[100][100];
  int completed[100] = {0};
  int tracker = 0;
  int input2;

  printf("\nWhat would you like to do?\n");
  printf("1. Add task\n2. View list\n3. Complete task\n4. Quit\n");
  scanf("%d", &input2);
  getchar(); // clears the buffer

  while (input2 != 4) {
    // add task
    if (input2 == 1) {
      printf("Enter item:\n");
      fgets(toDoList[tracker], 100, stdin);
      tracker++;
    }

    // view to do list
    else if (input2 == 2) {
      printf("\nTo do:\n");
      {
        for (int i = 0; i < tracker; i++)
          if (completed[i] == 0)
            printf(" %d) %s", i + 1, toDoList[i]);
          else
            printf(" %d) \033[9m%s\033[0m", i + 1, toDoList[i]);
      }
    }

    // complete task
    else if (input2 == 3) {
      printf(
          "Which task did you complete? (enter number of item on the list)\n");
      int task;
      scanf("%d", &task);
      task--;
      completed[task] = 1;
    }

    printf("\nWhat would you like to do?\n");
    printf("1. Add task\n2. View list\n3. Complete task\n4. Quit\n");
    scanf("%d", &input2);
    getchar();
  }
}

int main() {
  char topic[20];
  int input;

  while (input != 4) {
    printf("What would you like to do?\n");
    printf("\n1. Timer\n2. Alarm\n3. To-do-List\n4. Quit\n");
    scanf("%d", &input);
    // timer starts
    if (input == 1) {
      timer(topic);
    }
    // Alarm
    if (input == 2) {
      alarmFunction();
    }
    // To-do
    if (input == 3) {
      toDoList();
    }
  }
  return 0;
}
