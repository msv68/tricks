/**
 * The game.
 * We have 3 rooms. On each prob a bonus is placed in one room, two others remain empty.
 * We choose a room. Then one of the remaining rooms, which is empty, is open
 * to show there is no bonus in it.
 * Now we have the right to leave our first choice or change it, and choose another closed yet room.
 * The chosen room is open and we see if we win the bonus or lose.
 * 
 * The program has options:
 * -n <number>   defines the number of probs (default is 100);
 * -change       applies the right to change the chosen room;
 * -nochange     do not use the right to change the chosen room (default).
 * 
 * It outputs the percentage of wins.
**/

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <random>

int main (int theNbArgs, char** theArgs)
{
  long aNbProbs = 100;
  bool toChange = false;
  for (int i = 1; i < theNbArgs; ++i)
  {
    std::string anArg (theArgs[i]);
    std::transform(anArg.begin(), anArg.end(), anArg.begin(), std::tolower);
    if (anArg == "-n" && ++i < theNbArgs)
    {
      aNbProbs = std::stol (std::string (theArgs[i]));
    }
    else if (anArg == "-change")
    {
      toChange = true;
    }
    else if (anArg == "-nochange")
    {
      toChange = false;
    }
    else
    {
      std::cerr << "invalid usage" << std::endl;
      return 1;
    }
  }
  //std::cout << aNbProbs << " probs" << std::endl;

  std::random_device aDevice;
  std::mt19937 anEngine (aDevice());
  using uint_type = std::mt19937::result_type;
  std::uniform_int_distribution<uint_type> aDistr02 (0,2); // distribution in range [0, 2]
  uint_type aRooms[3];

  long aNbLuck = 0;
  for (long aProbNum = 0; aProbNum < aNbProbs; ++aProbNum)
  {
    // empty rooms
    std::fill (aRooms, aRooms+3, 0);
    
    // place bonus in one room
    uint_type aBonusRoom = aDistr02 (anEngine);
    aRooms[aBonusRoom] = 1;
    //std::cout << "-------------------------" << std::endl;
    //std::cout << "fill: " << aRooms[0] << " " << aRooms[1] << " " << aRooms[2] << std::endl;

    // choose a room randomly
    uint_type aChosenRoom = aDistr02 (anEngine);
    //std::cout << "chosen room is " << aChosenRoom << std::endl;

    // show empty room
    uint_type anEmptyRoom = ((aChosenRoom + 1) % 3 != aBonusRoom
                             ? (aChosenRoom + 1) % 3
                             : (aChosenRoom + 2) % 3);
    //std::cout << "empty room is " << anEmptyRoom << std::endl;

    // apply the right to change the choice
    uint_type aRoomToOpen = aChosenRoom;
    if (toChange)
    {
      aRoomToOpen = ((aChosenRoom + 1) % 3 != anEmptyRoom
                     ? (aChosenRoom + 1) % 3
                     : (aChosenRoom + 2) % 3);
    }

    // count wins
    if (aRooms[aRoomToOpen] == 1)
    {
      ++aNbLuck;
    }
    //std::cout << "we opened " << aRoomToOpen << ", and found " << aRooms[aRoomToOpen] << std::endl;
  }
  std::cout << "we were lucky " << aNbLuck << " times, it is " << 100 * aNbLuck / aNbProbs << "%" << std::endl;
}
