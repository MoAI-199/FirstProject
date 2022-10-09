#include "Server.h"
#include "Application.h"
#include "Network.h"
#include "Data.h"
#include "Exception.h"
#include "DxLib.h"

const int SETTING_NUM = 10;
const int CONDITION_TIME = 1200;


Server::Server( ) :
_recv_tcp_data( DataPtr( ) ) {
	const int num = ( int )_machine.size( );
	for ( int i = 0; i < num; i++ ) {
		_machine[ i ] = -1;
	}

	PreparationListenNetWork( TCP_PORT_NUM );
	_udp_handle = MakeUDPSocket( -1 );
	_condition_time = 0;
	_target_machine = 0;
}

Server::~Server( ) {
	DeleteUDPSocket( _udp_handle );
	StopListenNetWork( );
}

void Server::update( ) {
	listenForAcceptNewClient( );
	listenForPackets( );
}

std::string Server::getClientIPStr( int index ) const {
	std::string str;
	if ( _machine[ index ] >= 0 ) {
		IPDATA ip;
		GetNetWorkIP( _machine[ index ], &ip );
		str += std::to_string( ( ULONGLONG )ip.d1 );
		str += ".";
		str += std::to_string( ( ULONGLONG )ip.d2 );
		str += ".";
		str += std::to_string( ( ULONGLONG )ip.d3 );
		str += ".";
		str += std::to_string( ( ULONGLONG )ip.d4 );
	}
	return str;
}

std::string Server::getServerIPStr( ) const {
	IPDATA data;
	GetMyIPAddress( &data );

	return
		std::to_string( data.d1 ) + "." +
		std::to_string( data.d2 ) + "." +
		std::to_string( data.d3 ) + "." +
		std::to_string( data.d4 );
}

void Server::listenForAcceptNewClient( ) {
	int handle = GetNewAcceptNetWork( );
	if ( handle == -1 ) {
		return;
	}
	
	for ( int i = 0; i < MAX_MACHINES; i++ ) {
		if ( _machine[ i ] == -1 ) {
			_machine[ i ] = handle;
			break;
		}
	}
}

int Server::getRecievingIdx( ) const {
	return _recieving_idx;
}

void Server::listenForPackets( ) {
	lostMachine( );
	recieveTcp( );
	sendCondition( );
}

void Server::lostMachine( ) {
	int handle = GetLostNetWork( );

	for ( int i = 0; i < MAX_MACHINES; i++ ) {
		if ( _machine[ i ] == -1 ) {
			continue;
		}

		if ( handle == _machine[ i ] ) {
			CloseNetWork( _machine[ i ] );
			_machine[ i ] = -1;
			continue;
		}
	}
}

void Server::recieveTcp( ) {
	_recieving_idx = -1;
	for ( int i = 0; i < MAX_MACHINES; i++ ) {
		//一度受信したら、その次のマシンからチェックするようにする
		int target_machine = ( i + _target_machine ) % MAX_MACHINES;
		if ( _machine[ target_machine ] == -1 ) {
			continue;
		}
		if ( GetNetWorkDataLength( _machine[ target_machine ] ) <= 0 ) {
			continue;
		}
		// 受信
		EXCEPTION( _recv_tcp_data == DataPtr( ), "not set _recv_tcp_data" );
		if ( NetWorkRecv( _machine[ target_machine ], _recv_tcp_data->getPtr( ), _recv_tcp_data->getSize( ) ) == 0 ) {
			_recieving_idx = target_machine;
			_target_machine = target_machine;
			break;
		}

	}
}

void Server::saveIP( ) {
	IPDATA data;
	GetMyIPAddress( &data );

	FILE* fp;
	fopen_s( &fp, "IP.ini", "wb" );
	fwrite( &data, sizeof( IPDATA ), 1, fp );
	fclose( fp );
}

void Server::setReceiveTcp( DataPtr data ) {
	_recv_tcp_data = data;
}

void Server::sendUdp( DataPtr data ) {
	for ( int i = 0; i < MAX_MACHINES; i++ ) {
		if ( _machine[ i ] < 0 ) {
			continue;
		}
		IPDATA ip;
		GetNetWorkIP( _machine[ i ], &ip );
		NetWorkSendUDP( _udp_handle, ip, UDP_PORT_NUM, data->getPtr( ), data->getSize( ) ) ;
	}
}

void Server::sendTcp( DataPtr data ) {
	for ( int i = 0; i < MAX_MACHINES; i++ ) {
		if ( _machine[ i ] < 0 ) {
			continue;
		}
		NetWorkSend( _machine[ i ], data->getPtr( ), data->getSize( ) );
	}
}

void Server::sendCondition( ) {
	// 一定時間
	_condition_time++;
	if ( _condition_time < CONDITION_TIME ) {
		return;
	}
	_condition_time = 0;

	// 送信
	for ( int i = 0; i < MAX_MACHINES; i++ ) {
		if ( _machine[ i ] < 0 ) {
			continue;
		}

		char fourcc[ 4 ] = { 'C', 'O', 'N', 'D' };

		if ( NetWorkSend( _machine[ i ], ( void * )fourcc, sizeof( fourcc ) ) < 0 ) {
			CloseNetWork( _machine[ i ] );
			_machine[ i ] = -1;
		}
	}
}

