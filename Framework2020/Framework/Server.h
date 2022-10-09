#pragma once

#include "Task.h"
#include "smart_ptr.h"
#include <array>

PTR( Server );
PTR( Data );

class Server : public Task {
public:
	static const int MAX_MACHINES = 20;
public:
	Server( );
	virtual ~Server( );
public:
	void saveIP( );
	std::string getClientIPStr( int index ) const;
	std::string getServerIPStr( ) const;
	void setReceiveTcp( DataPtr data );
	void sendUdp( DataPtr data );
	void sendTcp( DataPtr data );
	int getRecievingIdx( ) const;
public:
	virtual void update( ) override;
private:
	void listenForAcceptNewClient( );
	void listenForPackets( );
	void lostMachine( );
	void recieveTcp( );
	void sendCondition( );
private:
	std::array< int, MAX_MACHINES > _machine;
	int _udp_handle;
	DataPtr _recv_tcp_data;
	int _recieving_idx;
	int _condition_time;
	int _target_machine;
};