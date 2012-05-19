/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: George F. Riley<riley@ece.gatech.edu>
 * Modified by: John Abraham <john.abraham@gatech.edu>
 */

// Interface between ns3 and the network animator

#ifndef ANIMATION_INTERFACE__H
#define ANIMATION_INTERFACE__H

#include <string>
#include <stdio.h>
#include <map>
#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/node-container.h"
#include "ns3/nstime.h"
#include "ns3/log.h"
#include "ns3/node-list.h"
#include "ns3/simulator.h"
#include "ns3/config.h"
#include "ns3/animation-interface-helper.h"
#include "ns3/mac48-address.h"

#ifdef WIN32
#include <winsock2.h>
#include <io.h>
#define STDOUT_FILENO (SOCKET)GetStdHandle(STD_OUTPUT_HANDLE)
#define close _close
#define write _write
#undef GetObject
#undef min
#undef max
#define HANDLETYPE SOCKET
#else
#include "ns3/netanim-config.h"
#define HANDLETYPE int
#endif

namespace ns3 {

#define MAX_PKTS_PER_TRACE_FILE 100000

/**
 * \defgroup netanim Netanim
 *
 * This section documents the API of the ns-3 netanim module. For a generic functional description, please refer to the ns-3 manual.
 */

/**
 * \ingroup netanim
 *
 * \brief Interface to network animator
 *
 * Provides functions that facilitate communications with an
 * external or internal network animator.
 */
class AnimationInterface
{
public:

  /**
   * \brief Construct the animator interface. No arguments needed.
   *
   */
  AnimationInterface ();

  /**
   * \brief Destructor for the animator interface.
   *
   */
  ~AnimationInterface ();

  /**
   * \brief Constructor
   * \param filename The Filename for the trace file used by the Animator
   * \param maxPktsPerFile The maximum number of packets per trace file.
	    AnimationInterface will create trace files with the following 
            filenames : filename, filename-1, filename-2..., filename-N
	    where each file contains packet info for 'maxPktPerFile' number of packets
   * \param usingXML Set to true if XML output traces are required
   *
   */
  AnimationInterface (const std::string filename, 
	uint64_t maxPktsPerFile = MAX_PKTS_PER_TRACE_FILE, 
	bool usingXML = true);

  /**
   * \brief Constructor
   * \param port Port on which ns-3 should listen to for connection from the
   *        external netanim application
   * \param usingXML Set to true if XML output traces are required
   *
   */
  AnimationInterface (uint16_t port, bool usingXML = true);

  /**
   * \brief Check if AnimationInterface is initialized
   * \returns true if AnimationInterface was already initialized
   *
   */
  static bool IsInitialized (void);

  /**
   * \brief Specify that animation commands are to be written
   * to the specified output file.
   *
   * This call is used to write the animation information to a text
   * file that can later be used as input to the network animator tool.
   *
   * \param fn The name of the output file.
   * \returns true if successful open.
   *
   */
  bool SetOutputFile (const std::string& fn);

  /**
   * \brief Specify that animation commands are to be written
   * in XML format.
   *
   * \returns none
   *
   */
  void SetXMLOutput ();

  /**
   * \brief Specify the time at which capture should start
   * 
   * \param t The time at which AnimationInterface should begin capture of traffic info
   *
   * \returns none
   */
  void SetStartTime (Time t);

  /**
   * \brief Specify the time at which capture should stop
   * 
   * \param t The time at which AnimationInterface should stop capture of traffic info
   *
   * \returns none
   */
  void SetStopTime (Time t);

  /**
   * \brief (Deprecated) Specify that animation commands are to be written to
   * a socket.
   *
   * This call is used to set the ns3 process in server mode, waiting
   * for a TCP connection from the animator.  This call will not
   * return until the animator connects in, or if the bind to the
   * specified port fails.
   *
   * \param port Port number to bind to.
   * \returns true if connection created, false if bind failed.
   *
   */
  bool SetServerPort (uint16_t port);

  /**
   * \brief Writes the topology information and sets up the appropriate
   *  animation packet tx callback
   *
   * Writes the topology information to the appropriate output, depending
   * on prior calls to SetOutputFile, SetServerPort, or SetInternalAnimation.
   * Then creates the callbacks needed for the animator to start processing
   * packets.
   * 
   * \param restart True when restarting animation
   */
  void StartAnimation (bool restart = false);

  /**
   * \brief Closes the interface to the animator.
   *
   */
  void StopAnimation ();

  /**
   * \brief Set mobility poll interval:WARNING: setting a low interval can 
   * cause slowness
   *
   * \param t Time interval between fetching mobility/position information
   * Default: 0.25s
   *
   */
  void SetMobilityPollInterval (Time t);

  /**
   * \brief Set random position if a Mobility Model does not exists for the node
   *
   * \param setRandPos True if a random position can be set for a node without a
   * Mobililty model
   *
   */
  void SetRandomPosition (bool setRandPos);

  /**
   * \brief typedef for WriteCallBack used for listening to AnimationInterface
   * write messages
   * 
   */
  typedef void (*AnimWriteCallback) (const char * str);

  /**
   * \brief Set a callback function to listen to AnimationInterface write events
   *
   * \param cb Address of callback function
   *
   */
  void SetAnimWriteCallback (AnimWriteCallback cb);

  /**
   * \brief Reset the write callback function
   *
   */
  void ResetAnimWriteCallback ();

  /**
   * \brief Helper function to set Constant Position for a given node
   * \param n Ptr to the node
   * \param x X co-ordinate of the node
   * \param y Y co-ordinate of the node
   * \param z Z co-ordinate of the node
   *
   */
  static void SetConstantPosition (Ptr <Node> n, double x, double y, double z=0);

  /**
   * \brief Helper function to set a brief description for a given node
   * \param n Ptr to the node
   * \param descr A string to briefly describe the node
   *
   */
  static void SetNodeDescription (Ptr <Node> n, std::string descr);

  /**
   * \brief Helper function to set a brief description for nodes in a Node Container
   * \param nc NodeContainer containing the nodes
   * \param descr A string to briefly describe the nodes
   *
   */
  static void SetNodeDescription (NodeContainer nc, std::string descr);

  /**
   * \brief Is AnimationInterface started
   * \returns true if AnimationInterface was started
   *
   */
  bool IsStarted (void);

  /**
   * \brief Show all 802.11 frames. Default: show only frames accepted by mac layer
   * \param showAll if true shows all 802.11 frames including beacons, association
   *  request and acks (very chatty). if false only frames accepted by mac layer
   *
   */
  void ShowAll802_11 (bool showAll); 

  /**
   *
   * \brief Enable Packet metadata
   * \param enable if true enables writing the packet metadata to the XML trace file
   *        if false disables writing the packet metadata
   */
  void EnablePacketMetadata (bool enable);


  /**
   *
   * \brief Get trace file packet count (This used only for testing)
   *
   * returns Number of packets recorded in the current trace file
   *
   */
  uint64_t GetTracePktCount ();


private:
#ifndef WIN32
  int m_fHandle;  // File handle for output (-1 if none)
  // Write specified amount of data to the specified handle
  int WriteN (int, const char*, uint32_t);
#else
  SOCKET m_fHandle;  // File handle for output (-1 if none)
  int  WriteN (SOCKET, const char*, uint32_t);
#endif
  bool m_xml;      // True if xml format desired
  Time m_mobilityPollInterval;
  bool m_usingSockets;
  uint16_t m_port;
  std::string m_outputFileName;
  bool m_outputFileSet;
  bool m_serverPortSet;
  uint64_t gAnimUid ;    // Packet unique identifier used by Animtion
  bool m_randomPosition;
  AnimWriteCallback m_writeCallback;
  bool m_started;
  bool m_enablePacketMetadata; 
  Time m_startTime;
  Time m_stopTime;
  uint64_t m_maxPktsPerFile;
  std::string m_originalFileName;

  void DevTxTrace (std::string context,
                   Ptr<const Packet> p,
                   Ptr<NetDevice> tx,
                   Ptr<NetDevice> rx,
                   Time txTime,
                   Time rxTime);
  void WifiPhyTxBeginTrace (std::string context,
                            Ptr<const Packet> p);
  void WifiPhyTxEndTrace (std::string context,
                          Ptr<const Packet> p);
  void WifiPhyTxDropTrace (std::string context,
                           Ptr<const Packet> p);
  void WifiPhyRxBeginTrace (std::string context,
                            Ptr<const Packet> p);
  void WifiPhyRxEndTrace (std::string context,
                          Ptr<const Packet> p);
  void WifiMacRxTrace (std::string context,
                       Ptr<const Packet> p);
  void WifiPhyRxDropTrace (std::string context,
                           Ptr<const Packet> p);
  void WimaxTxTrace (std::string context,
                     Ptr<const Packet> p,
		     const Mac48Address &);
  void WimaxRxTrace (std::string context,
                     Ptr<const Packet> p,
                     const Mac48Address &);
  void CsmaPhyTxBeginTrace (std::string context,
                            Ptr<const Packet> p);
  void CsmaPhyTxEndTrace (std::string context,
                            Ptr<const Packet> p);
  void CsmaPhyRxEndTrace (std::string context,
                          Ptr<const Packet> p);
  void CsmaMacRxTrace (std::string context,
                       Ptr<const Packet> p);

  void LteTxTrace (std::string context,
                      Ptr<const Packet> p,
                      const Mac48Address &);

  void LteRxTrace (std::string context,
                      Ptr<const Packet> p,
                      const Mac48Address &);

  void MobilityCourseChangeTrace (Ptr <const MobilityModel> mob);

  // Write a string to the specified handle;
  int  WriteN (int, const std::string&);

  void OutputWirelessPacket (Ptr<const Packet> p, AnimPacketInfo& pktInfo, AnimRxInfo pktrxInfo);
  void OutputCsmaPacket (Ptr<const Packet> p, AnimPacketInfo& pktInfo, AnimRxInfo pktrxInfo);
  void MobilityAutoCheck ();
  

  std::map<uint64_t, AnimPacketInfo> m_pendingWifiPackets;
  void AddPendingWifiPacket (uint64_t AnimUid, AnimPacketInfo&);
  bool WifiPacketIsPending (uint64_t AnimUid); 

  std::map<uint64_t, AnimPacketInfo> m_pendingWimaxPackets;
  void AddPendingWimaxPacket (uint64_t AnimUid, AnimPacketInfo&);
  bool WimaxPacketIsPending (uint64_t AnimUid); 

  std::map<uint64_t, AnimPacketInfo> m_pendingLtePackets;
  void AddPendingLtePacket (uint64_t AnimUid, AnimPacketInfo&);
  bool LtePacketIsPending (uint64_t AnimUid);

  std::map<uint64_t, AnimPacketInfo> m_pendingCsmaPackets;
  void AddPendingCsmaPacket (uint64_t AnimUid, AnimPacketInfo&);
  bool CsmaPacketIsPending (uint64_t AnimUid);

  uint64_t GetAnimUidFromPacket (Ptr <const Packet>);

  std::map<uint32_t, Vector> m_nodeLocation;
  Vector GetPosition (Ptr <Node> n);
  Vector UpdatePosition (Ptr <Node> n);
  Vector UpdatePosition (Ptr <Node> n, Vector v);
  void WriteDummyPacket ();
  bool NodeHasMoved (Ptr <Node> n, Vector newLocation);
  void AddMargin ();

  void PurgePendingWifi ();
  void PurgePendingWimax ();
  void PurgePendingLte ();
  void PurgePendingCsma ();

  // Recalculate topology bounds
  void RecalcTopoBounds (Vector v);
  std::vector < Ptr <Node> > RecalcTopoBounds ();

  void ConnectCallbacks ();

  
  std::map <std::string, uint32_t> m_macToNodeIdMap;
  bool IsInTimeWindow ();

  // Path helper
  std::vector<std::string> GetElementsFromContext (std::string context);
  Ptr <NetDevice> GetNetDeviceFromContext (std::string context);

  static std::map <uint32_t, std::string> nodeDescriptions;
  uint64_t m_currentPktCount;

  void StartNewTraceFile();

  // XML helpers
  std::string GetPreamble (void);
  // Topology element dimensions
  double m_topoMinX;
  double m_topoMinY;
  double m_topoMaxX;
  double m_topoMaxY;

  std::string GetPacketMetadata (Ptr<const Packet> p);

  std::string GetXMLOpen_anim (uint32_t lp);
  std::string GetXMLOpen_topology (double minX,double minY,double maxX,double maxY);
  std::string GetXMLOpenClose_node (uint32_t lp,uint32_t id,double locX,double locY);
  std::string GetXMLOpenClose_link (uint32_t fromLp,uint32_t fromId, uint32_t toLp, uint32_t toId);
  std::string GetXMLOpen_packet (uint32_t fromLp,uint32_t fromId, double fbTx, double lbTx, std::string auxInfo = "");
  std::string GetXMLOpenClose_rx (uint32_t toLp, uint32_t toId, double fbRx, double lbRx);
  std::string GetXMLOpen_wpacket (uint32_t fromLp,uint32_t fromId, double fbTx, double lbTx, double range);
  std::string GetXMLClose (std::string name) {return "</" + name + ">\n"; }
  std::string GetXMLOpenClose_meta (std::string metaInfo);

};

/**
 * \ingroup netanim
 *
 * \brief Byte tag using by Anim to uniquely identify packets
 *
 * When Anim receives a Tx Notification we tag the packet with a unique global uint64_t identifier
 * before recording Tx information
 * When Anim receives Rx notifications the tag is used to retrieve Tx information recorded earlier 
 * 
 */

class AnimByteTag : public Tag
{
public:

  /**
   * \brief Get Type Id
   * \returns Type Id
   *
   */
  static TypeId GetTypeId (void);
  
  /**
   * \brief Get Instance Type Id
   * \returns Type Id
   *
   */
  virtual TypeId GetInstanceTypeId (void) const;

  /**
   * \brief Get Serialized Size
   * \returns Serialized Size (i.e size of uint64_t)
   *
   */
  virtual uint32_t GetSerializedSize (void) const;

  /**
   * \brief Serialize function
   * \param i Tag Buffer
   *
   */
  virtual void Serialize (TagBuffer i) const;

  /**
   * \brief Deserialize function
   * \param i Tag Buffer
   *
   */
  virtual void Deserialize (TagBuffer i);

  /**
   * \brief Print tag info
   * \param os Reference of ostream object
   *
   */
  virtual void Print (std::ostream &os) const;

  /**
   * \brief Set global Uid in tag
   * \param AnimUid global Uid
   *
   */
  void Set (uint64_t AnimUid);

  /**
   * \brief Get Uid in tag
   * \returns Uid in tag
   *
   */
  uint64_t Get (void) const;

private:
  uint64_t m_AnimUid;
};

}
#endif

