#pragma once

#include "smart_ptr.h"
#include "Task.h"
#include <string>

PTR( Client );
PTR( Device );
PTR( Sound );
PTR( Inputter );
PTR( Data );

class Client : public Task {
public:
	Client( );
	virtual ~Client( );
public:
	virtual void update( );
public:
	void setRecvDataTcp( DataPtr data );
	void setRecvDataUdp( DataPtr data );
	std::string getPhase( ) const;
	void sendTcp( DataPtr data );
	bool isRecievingUDP( ) const;
	bool isRecievingTCP( ) const;
	std::string getServerIPStr( ) const;
	std::string getClientIPStr( ) const;
private:
	enum PHASE {
		PHASE_READY,
		PHASE_CONNECTING,
	};
private:
	void updateReady( );
	void updateConnecting( );
	/// <summary>
	/// IPÇèëÇ´èoÇ∑
	/// </summary>
	/// <returns> trueÇ©false </returns>
	bool loadIP( );
	bool connect( );
	void recieveUdp( );
	void recieveTcp( );
private:
	DataPtr _recv_udp_data;
	DataPtr _recv_tcp_data;
	PHASE _phase;
	unsigned char _ip[ 4 ];
	int _tcp_handle;
	int _udp_handle;
	int _network_state_time;
	bool _recieving_udp;
	bool _recieving_tcp;
};

