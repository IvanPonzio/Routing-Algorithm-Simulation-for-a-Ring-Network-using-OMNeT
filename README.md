# Routing Algorithm Design for a Network

## Group Members
- Fernando Alvarado
- Ivan Ponzio
- Armando Carral

## Summary
This report focuses on addressing congestion issues caused by routing failures in a network. We worked with an 8-node ring network as a specific case, where each node had application, network, and link layers with two interfaces towards neighboring nodes. The project involved two parts: initially, we analyzed the network without applying any algorithm, considering different metrics for subsequent analysis and understanding the function of each layer in the network. Then, we designed and implemented a simplified algorithm based on OSPF (Open Shortest Path First) specifically tailored to our network design. We repeated the same cases as in the first part, but this time with the implemented algorithm. We analyzed and compared the obtained results with those from the first part.

## Methods
The implemented algorithm is a simplified variant of OSPF. While OSPF relies on a more advanced and complex link-state approach, our algorithm utilizes a basic distance-vector approach. We utilized two fixed-size arrays of size 8 (representing the routing table for each node) to calculate the cost of reaching the destination denoted by the index. We facilitated information exchange between network nodes using distance-vector messages. We created custom getters and setters in the network layer to construct and read these arrays. Similar to OSPF, our implemented algorithm utilizes messages to update the routing tables. Both algorithms consider link costs and next hops towards reachable destinations. The algorithm provides dynamic and efficient routing, adapting to changes in the network's topology. In case of a link failure or the addition of a new node, nodes update their routing tables through message exchanges, selecting the best routes based on link costs.

## Results
The results obtained from the simulation experiments demonstrated the effectiveness of the implemented routing algorithm. By utilizing the distance-vector approach and considering link costs, the algorithm successfully reduced congestion and improved packet delivery in the network. Detailed analysis and comparison of the results between the cases without and with the routing algorithm highlighted the algorithm's positive impact on network performance.

## Conclusion
In conclusion, the designed and implemented routing algorithm effectively addressed congestion issues in the network caused by routing failures. Leveraging the simplified distance-vector approach and considering link costs, the algorithm dynamically updated routing tables, adapting to changes in the network's topology.

**Note**: This readme provides a brief overview of the project. For more detailed information, including implementation details and analysis, please refer to the complete report.

*This project was conducted as part of the research activities at FAMAF (Facultad de Matemática, Astronomía, Física y Computación). The OMNeT++ network simulation framework was utilized for simulation and evaluation purposes.*
