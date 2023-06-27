#ifndef DISTANCEVECTORMSG_H
#define DISTANCEVECTORMSG_H

#include <utility>
#include <omnetpp.h>


class DistanceVectorMsg : public omnetpp::cMessage {
private:
    int senderIndex;
    int senderLink;
    int distanceVector[8];  // Distance vector: Destination -> Cost
    int nextHop[8];  // Destination -> Next Hop


public:
    DistanceVectorMsg();
    virtual ~DistanceVectorMsg();

    int getSenderIndex() const;
    void setSenderIndex(int index);

    int getSenderLink() const;
    void setSenderLink(int link);

    int getDistanceVector(int destination) const;
    void setDistanceVector(int destination, int cost);

    int getNextHop(int destination) const;
    void setNextHop(int destination, int hop);

    DistanceVectorMsg* dup() const override { return new DistanceVectorMsg(*this); }
};

#endif  // DISTANCEVECTORMSG_H
