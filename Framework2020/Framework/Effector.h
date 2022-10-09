#pragma once
#include "Task.h"
#include "smart_ptr.h"
#include <string>
#include <map>

PTR( Effector );
PTR( Effect );

class Effector : public Task {
public:
	/// <summary> Effectorのコンストラクタ </summary>
	/// <param name="directory"> ファイル名 </param>
	Effector( const std::string& directory );
	/// <summary>
	/// Effectorのデストラクタ
	/// </summary>
	virtual ~Effector( );
public:
	/// <summary>
	/// エフェクトを作成する
	/// </summary>
	EffectPtr createEffect( const std::string& filename ); // ロードしたエフェクトのidを返す
	/// <summary>
	/// エフェクトを描画する
	/// </summary>
	void drawEffect( ) const;
private:
	/// <summary> ファイル保存用変数 </summary>
	std::string _directory;
	/// <summary>
	/// リソースを扱う変数
	/// </summary>
	std::map< std::string, int > _resource_handles;
	/// <summary> コンストラクタ時に-1を代入する変数 </summary>
	int _effekseer_graph;
};

