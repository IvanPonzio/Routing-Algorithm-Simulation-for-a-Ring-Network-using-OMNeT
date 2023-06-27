#ifndef APP
#define APP

#include <string.h>
#include <omnetpp.h>
#include <packet_m.h>

using namespace omnetpp;

class App: public cSimpleModule {
private:
    cMessage *sendMsgEvent;
    cStdDev delayStats;
    cOutVector delayVector;
    cOutVector paquetesEnviados;
    cOutVector paquetesRecibidos;
public:
    App();
    virtual ~App();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
    std::vector<cOutVector*> delayVectors; // Vector de punteros a cOutVector
 // Vector de vectores de retardo

};

Define_Module(App);

#endif /* APP */

App::App() {
}

App::~App() {
}

void App::initialize() {

    // If interArrivalTime for this node is higher than 0
    // initialize packet generator by scheduling sendMsgEvent
    if (par("interArrivalTime").doubleValue() != 0) {
        sendMsgEvent = new cMessage("sendEvent");
        scheduleAt(par("interArrivalTime"), sendMsgEvent);
    }
    paquetesEnviados.setName("Paquetes enviados");
    paquetesRecibidos.setName("Paquetes Recibidos");
    // Initialize statistics
    delayStats.setName("TotalDelay");
    delayVector.setName("Delay");

    int numNodes = 8; // Número total de nodos en el anillo
    delayVectors.resize(numNodes, nullptr); // Redimensionar el vector de punteros a cOutVector según el número de nodos

    // Inicializar los punteros a cOutVector
    for (int i = 0; i < numNodes; i++) {
        char vectorName[20];
        sprintf(vectorName, "DelayNodo%d", i); // Nombre del vector de retardo para el nodo i
        delayVectors[i] = new cOutVector(vectorName);
    }
}

void App::finish() {
    // Record statistics
    recordScalar("Average delay", delayStats.getMean());
    recordScalar("Number of packets", delayStats.getCount());
    for (auto delayVector : delayVectors) {
        delete delayVector;
    }
}

void App::handleMessage(cMessage *msg) {

    // if msg is a sendMsgEvent, create and send new packet
    if (msg == sendMsgEvent) {
        // create new packet
        Packet *pkt = new Packet("packet",this->getParentModule()->getIndex());
        pkt->setByteLength(par("packetByteSize"));
        pkt->setSource(this->getParentModule()->getIndex());
        pkt->setDestination(par("destination"));

        // send to net layer
        send(pkt, "toNet$o");
        paquetesEnviados.record(1);
        // compute the new departure time and schedule next sendMsgEvent
        simtime_t departureTime = simTime() + par("interArrivalTime");
        scheduleAt(departureTime, sendMsgEvent);

    }
    // else, msg is a packet from net layer
    else {
           // compute delay and record statistics
           simtime_t delay = simTime() - msg->getCreationTime();
           delayStats.collect(delay);
           delayVector.record(1);
           int sourceNode = dynamic_cast<Packet*>(msg)->getSource();

              // Guardar el retardo en el vector correspondiente al nodo de origen
           delayVectors[sourceNode]->record(delay);
           paquetesRecibidos.record(1);
           // delete msg
           delete msg;
       }

}
