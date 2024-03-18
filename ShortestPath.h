#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H

#include "MapParser.h"
#include "CollisionHandler.h"
#include <vector>
#include <queue>
#include <cmath>
#include "Engine.h"
#include <algorithm>


class ShortestPath {
    public:
        inline static ShortestPath* GetInstance() {
            if (s_Instance != nullptr)
                return s_Instance;
            else
                return s_Instance = new ShortestPath();
        }

        int checkKD(Point a) {
            if (a.X >= 0 && a.X <= 60 && a.Y <= 5) return 1;
            if (a.X >= 0 && a.X <= 60 && a.Y <= 12 && a.Y >= 6) return 2;
            //if (a.X >= 19 && a.X <= 30 && a.Y <= 4) return 1;
            //if (a.X >= 10 && a.X <= 39  && a.Y <= 11 && a.Y >= 6) return 2;
            //if (a.X >= 0 && a.X <= 60 && a.Y >= 12 && a.Y <= 20 ) return 3;
            return 3;
        }

        std::vector<Point> cungKhuDat(Point m, Point e) {
            std::vector<Point> dir;
            if (checkKD(m) == checkKD(e)) return dir;
            else {
                
                if (checkKD(e) == 1 && checkKD(m) == 2) {
                    float kc1 = Point::KC(e, K1[3]) + Point::KC(K2[4],m);
                    float kc2 = Point::KC(e, K1[4]) + Point::KC(K2[5], m);
                    
                    if (kc1 > kc2) {
                        dir.push_back(K1[4]);
                        dir.push_back(K1[5]);
                        dir.push_back(K2[5]);
                    }
                    else {
                        dir.push_back(K1[3]);
                        dir.push_back(K1[2]);
                        dir.push_back(K2[4]);
                    }
                }
                else if (checkKD(e) == 2 && checkKD(m) == 1) {
                    float kc1 = Point::KC(e, K2[4]) + Point::KC(K1[3], m);
                    float kc2 = Point::KC(e, K2[5]) + Point::KC(K1[4], m);
                    if (kc1 > kc2) {
                        dir.push_back(K2[5]);
                        dir.push_back(K1[6]);
                        dir.push_back(K1[7]);
                        dir.push_back(K1[4]);
                    }
                    else {
                        dir.push_back(K2[4]);
                        dir.push_back(K1[0]);
                        dir.push_back(K1[1]);
                        dir.push_back(K1[3]);
                    }
                }
                else if (checkKD(e) == 2 && checkKD(m) == 3) {
                    float kc1 = Point::KC(e, K2[2]) + Point::KC(K3[3],m);
                    float kc2 = Point::KC(e, K2[6]) + Point::KC(K3[4], m);
                    if (kc1 > kc2) {
                        dir.push_back(K2[6]);
                        dir.push_back(K2[9]);
                        dir.push_back(K3[4]);
                    }
                    else {
                        dir.push_back(K2[2]);
                        dir.push_back(K2[3]);
                        dir.push_back(K3[3]);
                    }
                    }
                else if (checkKD(e) == 3 && checkKD(m) == 2) {
                    if (e.X < 5 && e.Y < 13) {
                        dir.push_back(K3[1]);
                    }

                    float kc1 = Point::KC(e, K3[3]) + Point::KC(K2[2], m);
                    float kc2 = Point::KC(e, K3[4]) + Point::KC(K2[6], m);
                    
                    if (kc1 > kc2) {
                        dir.push_back(K3[4]);
                        dir.push_back(K2[8]);
                        dir.push_back(K2[7]);
                        dir.push_back(K2[6]);
                    }
                    else {
                        dir.push_back(K3[3]);
                        dir.push_back(K2[0]);
                        dir.push_back(K2[1]);
                        dir.push_back(K2[2]);
                    }
                }
                else if (checkKD(e) == 3 && checkKD(m) == 1) {
                    if (e.X < 5 && e.Y < 13) {
                        dir.push_back(K3[1]);
                    }

                    float kc1 = Point::KC(e, K3[3]) + Point::KC(K2[2], m);
                    float kc2 = Point::KC(e, K3[4]) + Point::KC(K2[6], m);

                    if (kc1 > kc2) {
                        dir.push_back(K3[4]);
                        dir.push_back(K2[8]);
                        dir.push_back(K2[7]);
                        dir.push_back(K2[6]);
                    }
                    else {
                        dir.push_back(K3[3]);
                        dir.push_back(K2[0]);
                        dir.push_back(K2[1]);
                        dir.push_back(K2[2]);
                    }

                    kc1 = Point::KC(e, K2[4]) + Point::KC(K1[3], m);
                    kc2 = Point::KC(e, K2[5]) + Point::KC(K1[4], m);
                    if (kc1 > kc2) {
                        dir.push_back(K2[5]);
                        dir.push_back(K1[6]);
                        dir.push_back(K1[7]);
                        dir.push_back(K1[4]);
                    }
                    else {
                        dir.push_back(K2[4]);
                        dir.push_back(K1[0]);
                        dir.push_back(K1[1]);
                        dir.push_back(K1[3]);
                    }
                }
                else if (checkKD(e) == 1 && checkKD(m) == 3) {
                    float kc1 = Point::KC(e, K1[3]) + Point::KC(K2[4], m);
                    float kc2 = Point::KC(e, K1[4]) + Point::KC(K2[5], m);

                    if (kc1 > kc2) {
                        dir.push_back(K1[4]);
                        dir.push_back(K1[5]);
                        dir.push_back(K2[5]);
                    }
                    else {
                        dir.push_back(K1[3]);
                        dir.push_back(K1[2]);
                        dir.push_back(K2[4]);
                    }

                    kc1 = Point::KC(e, K2[2]) + Point::KC(K3[3], m);
                    kc2 = Point::KC(e, K2[6]) + Point::KC(K3[4], m);
                    if (kc1 > kc2) {
                        dir.push_back(K2[6]);
                        dir.push_back(K2[9]);
                        dir.push_back(K3[4]);
                    }
                    else {
                        dir.push_back(K2[2]);
                        dir.push_back(K2[3]);
                        dir.push_back(K3[3]);
                    }
                }
                for (int i = 0; i < dir.size(); i++) {
                    dir[i].X = dir[i].X * 32 - 64;
                    dir[i].Y = dir[i].Y * 32 - 75;
                }
                return dir;
            }
        }
        

    public:
        static ShortestPath* s_Instance;
        std::vector<Point> K1 = { Point(19,3), Point(22,3), Point(19,4), Point(22,4), Point(27,4), Point(30,4), Point (30,3), Point (27,3)};
        std::vector<Point> K2 = { Point(10,9), Point(13,9), Point(13,10), Point(10,10), Point(19,10), Point(30,10),Point(36,10), Point(36,9), Point(39,9), Point(39,10) };
        std::vector<Point> K3 = { Point(2,13), Point(5,13), Point(5,16), Point(10,16), Point(39,16), Point(48,16) };
    };


#endif // SHORTESTPATH_H