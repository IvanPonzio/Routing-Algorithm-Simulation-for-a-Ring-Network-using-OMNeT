#ifndef NET
#define NET

#include <string.h>
#include <omnetpp.h>
#include <packet_m.h>
#include <unordered_map>
#include "DistanceVectorMsg.h"

using namespace omnetpp;

class Net: public cSimpleModule {
private:
    cStdDev hopCountStats;
    cOutVector hopCountVector;  // vector contador
    int distanceVector[8];  // Distance vector: Destination -> Cost
    int nextHop[8];  // Destination -> Next Hop
    int nextLink[8];

public:
    Net();
    virtual ~Net();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
    void sendDistanceVector();

};

Define_Module(Net);

#endif /* NET */

Net::Net() {
}

Net::~Net() {
}

void Net::initialize() {
    // inicializamos los saltos
    hopCountStats.setName("TotalHopCount");
    hopCountVector.setName("HopCount");

    // Initialize distance vector
    int nodeIndex = getParentModule()->getIndex();
    for (int i = 0; i < 8; i++) {
        if (i != nodeIndex) {
            distanceVector[i] = INT_MAX;  // Initialize cost as infinity
            nextHop[i] = -1;
            nextLink[i] = -1;
        }
    }
    // Schedule periodic distance vector updates
    scheduleAt(0, new cMessage("updateDistanceVector"));

}

void Net::finish() {
    recordScalar("Average Hop", hopCountStats.getMean());
    recordScalar("Number of packets", hopCountStats.getCount());
}

void Net::handleMessage(cMessage *msg) {
    if(strcmp(msg->getName(), "updateDistanceVector") == 0){
        sendDistanceVector();
        scheduleAt(simTime() + 1.0, msg);  // Schedule next update after 1 second
    }
    else{
        // Check if the received message is a distance vector message
        DistanceVectorMsg *dvMsg = dynamic_cast<DistanceVectorMsg*>(msg);
        if (dvMsg){
            int neighborIndex = dvMsg->getSenderIndex();
            int link = dvMsg->getArrivalGate()->getIndex();
            // Update distance vector based on received information
            for (int destination = 0; destination < 8; ++destination) {
                int cost = dvMsg->getDistanceVector(destination);
                if (cost < distanceVector[destination] && cost != INT_MAX) {
                    ++cost;                  // Add cost to reach the neighbor
                    distanceVector[destination] = cost;
                    nextHop[destination] = neighborIndex;
                    nextLink[destination] = link;
                }
            }
            delete msg;
        }
        else{

            Packet *pkt = (Packet *) msg;
            EV_INFO << "Valor de SALTOS: " << pkt->getHopCount() << endl;
            // Obtenemos el indice del node
            int nodeIndex = this->getParentModule()->getIndex();

            // If this node is the final destination, send to App
            if (pkt->getDestination() == nodeIndex) {
                //pkt->setHopCount(pkt->getHopCount() + 1);   //contador de salto
                hopCountStats.collect(pkt->getHopCount());
                hopCountVector.record(pkt->getHopCount());

                send(msg, "toApp$o");
            }
            // If not, forward the packet to some else... to who?
            else {
                //pkt->setHopCount(pkt->getHopCount() + 1);   //contador de salto

        //        send(msg, "toLnk$o", 0);

                int link = nextLink[pkt->getDestination()];
                if (link != -1) {
                    send(pkt, "toLnk$o", link);
                }
                else {
                    EV_WARN << "No next hop found for destination " << pkt->getDestination() << endl;
                    delete pkt;
                }

        //        if(nodeIndex == 2 || nodeIndex == 3 || nodeIndex == 4){
        //            send(msg, "toLnk$o", 1);
        //        }
        //        else {
        //            send(msg, "toLnk$o", 0);
        //        }
            }
        }
    }
}

void Net::sendDistanceVector() {
    // Create and send distance vector message to neighbors
    DistanceVectorMsg *dvMsg = new DistanceVectorMsg();
    int nodeIndex = getParentModule()->getIndex();
    dvMsg->setSenderIndex(nodeIndex);

    for (int destination = 0; destination < 8; ++destination) {
        int cost = distanceVector[destination];
        int hop = nextHop[destination];
        dvMsg->setDistanceVector(destination, cost);
        dvMsg->setNextHop(destination, hop);
    }

    send(dvMsg->dup(), "toLnk$o", 0);
    send(dvMsg, "toLnk$o", 1);

}



