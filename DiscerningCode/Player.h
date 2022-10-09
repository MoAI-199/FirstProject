/*
* �P�N���ɐ��삵���R�[�h�ɂȂ�܂��B
* �Ȃ̂ŁA���̎����Ȃ�u���������ȁv�Ƃ����R�����g������Ă��܂��B
* ���̃v���W�F�N�g�Ɣ�ׂĎ����̐����x��m���Ă�����������Ǝv���܂��B
* */

#pragma once
#include "smart_ptr.h"
#include "mathematics.h"
#include "Map.h"
#include "inside.h"

PTR( Player );
PTR( Map );
PTR( Inside );
PTR( Time );

class Player {
public:
	Player( MapPtr map, InsidePtr inside ,TimePtr time);
	virtual ~Player( );
public:
	Vector2 getPos( ) const;
	bool isClear( ) const;
	bool isGameOver( ) const;
	bool isScore( ) const;
public:
	/// <summary>
	/// �X�V
	/// </summary>
	void update( ) ;
	/// <summary>
	/// �������W�ݒ�
	/// </summary>
	void startPos( );
	/// <summary>
	/// �v���C���[�̓���
	/// ���P�_�Fpublic�ōs���K�v�������B�Ȃ�update�֐��ŌĂ΂Ȃ��̂���ł���B
	/// </summary>
	void move( );
private:
	/// <summary>
	/// �O�g�Ƃ̂����蔻����s���Ă���
	/// ���P�_�FPlayer�N���X�ł����蔻�����闝�R���s��
	/// </summary>
	void frameCollision( );
	/*�����̕ǂƂ̂����蔻����܂Ƃ߂Ă���B
	* ���P�_�F�R�[�f�B���O���[���u�֐��͓�������n�߂�v�ɑ����Ă��Ȃ��B
	* �����Ă�collisionInside()�ɕς���B*/
	void insideCollision( );
	/// <summary>
	/// �N���A�̏����𖞂�������N���A����
	/// ���P�_�F�P�֐��P�@�\�Ƃ���̂����z�Ȃ̂ŁA�������͕ʂ�isClear�Ȃǂƕʂ̊֐��ɂ���̂��悢�B
	/// </summary>
	void clear( );
	/// <summary>
	/// �ړ�����
	/// </summary>
	void moveLimit( );
	/// <summary>
	/// �u���b�N�P�P�̂����蔻�������Ă���B
	/// </summary>
	/// <param name="type">�ǂ̎��</param>
	/// <param name="status">�ǂ̏��</param>
	void collision( Map::WALL type, Inside::STATUS status );
private:
	Vector2 _pos;
	MapPtr _map;
	InsidePtr _inside;
	TimePtr _time;
private:
	bool _clear;
	bool _hit;
	bool _gameover;
	bool _score;
};

