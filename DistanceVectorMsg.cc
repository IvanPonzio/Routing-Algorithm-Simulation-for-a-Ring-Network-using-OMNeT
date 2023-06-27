#include "DistanceVectorMsg.h"


DistanceVectorMsg::DistanceVectorMsg() : cMessage() {
    senderIndex = -1;
    memset(distanceVector, 0, sizeof(distanceVector));
    memset(nextHop, 0, sizeof(nextHop));
}

DistanceVectorMsg::~DistanceVectorMsg() {
}

int DistanceVectorMsg::getSenderIndex() const {
    return senderIndex;
}

void DistanceVectorMsg::setSenderIndex(int index) {
    senderIndex = index;
}

int DistanceVectorMsg:: getSenderLink() const{
    return senderLink;
}
void DistanceVectorMsg:: setSenderLink(int link){
    senderLink = link;
}

int DistanceVectorMsg::getDistanceVector(int destination) const {
    return distanceVector[destination];
}
void DistanceVectorMsg::setDistanceVector(int destination, int cost) {
    distanceVector[destination] = cost;
}

int DistanceVectorMsg::getNextHop(int destination) const {
    return nextHop[destination];
}
void DistanceVectorMsg:: setNextHop(int destination, int hop){
   nextHop[destination] = hop;
}

