#ifndef LNK
#define LNK

#include <string.h>
#include <omnetpp.h>
#include <packet_m.h>
#include "DistanceVectorMsg.h"

using namespace omnetpp;

class Lnk: public cSimpleModule {
private:
    cQueue buffer;
    cMessage *endServiceEvent;
    simtime_t serviceTime;
    cOutVector bufferSizeVector;
    int maxBufferSize;  //variable para almacenar el tamaño maximo del buffer

public:
    Lnk();
    virtual ~Lnk();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Lnk);

#endif /* LNK */

Lnk::Lnk() {
    endServiceEvent = NULL;
}

Lnk::~Lnk() {
    cancelAndDelete(endServiceEvent);
}

void Lnk::initialize() {
    endServiceEvent = new cMessage("endService");
    bufferSizeVector.setName("Buffer Size");
    maxBufferSize = 0;  // inicializo en cero el valor del buffer

}

void Lnk::finish() {
    recordScalar("Max Buffer Size", maxBufferSize);  // Registra el tamaño máximo del buffer
}

void Lnk::handleMessage(cMessage *msg) {

    if (msg == endServiceEvent) {
        if (!buffer.isEmpty()) {
            // dequeue
            Packet* pkt = (Packet*) buffer.pop();
            bufferSizeVector.record(buffer.getLength());
            // send
            send(pkt, "toOut$o");
            serviceTime = pkt->getDuration();
            scheduleAt(simTime() + serviceTime, endServiceEvent);
        }
    } else { // msg is a packet
        if (msg->arrivedOn("toNet$i")) {
            // Check if the received message is a distance vector message
            DistanceVectorMsg *dvMsg = dynamic_cast<DistanceVectorMsg*>(msg);
            if (dvMsg){
                send(dvMsg, "toOut$o");
            }
            else{
                // enqueue
                buffer.insert(msg);
                bufferSizeVector.record(buffer.getLength());

                // Actualiza el tamaño maximo del buffer si es necesario
                if(buffer.getLength() > maxBufferSize){
                    maxBufferSize = buffer.getLength();
                }

                // if the server is idle
                if (!endServiceEvent->isScheduled()) {
                    // start the service now
                    scheduleAt(simTime() + 0, endServiceEvent);
                }
            }
        } else {
            // Check if the received message is a distance vector message
            DistanceVectorMsg *dvMsg = dynamic_cast<DistanceVectorMsg*>(msg);
            if (dvMsg){
                send(dvMsg, "toNet$o");
            }
            else{
            //msg is from out, send to net
                send(msg, "toNet$o");
            }
        }
    }
}
