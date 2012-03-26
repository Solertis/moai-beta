// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRENDERMGR_H
#define	MOAIRENDERMGR_H

#include <moaicore/MOAIEaseDriver.h>
#include <moaicore/MOAILua.h>

class MOAIProp;

//================================================================//
// MOAIRenderMgr
//================================================================//
/**	@name	MOAIRenderMgr
	@text	MOAIRenderMgr is responsible for drawing a list of MOAIRenderable
			objects. MOAIRenderable is the base class for any object that can be
			drawn. This includes MOAIProp and MOAILayer. To use MOAIRenderMgr
			pass a table of MOAIRenderable objects to MOAIRenderMgr.setRenderTable ().
			The table will usually be a stack of MOAILayer objects. The contents of
			the table will be rendered the next time a frame is drawn. Note that the
			table must be an array starting with index 1. Objects will be rendered
			counting from the base index until 'nil' is encountered. The render
			table may include other tables as entries. These must also be arrays
			indexed from 1.
*/
class MOAIRenderMgr :
	public MOAIGlobalClass < MOAIRenderMgr, MOAILuaObject > {
private:
	
	u32				mRenderCounter;	// increments every render
	MOAILuaRef		mRenderTable;
	
	//----------------------------------------------------------------//
	static int		_getRenderTable				( lua_State* L );
	static int		_setRenderTable				( lua_State* L );

	//----------------------------------------------------------------//
	void			RenderTable					( MOAILuaState& state, int idx );

public:

	DECL_LUA_SINGLETON ( MOAIRenderMgr )
	
	GET ( u32, RenderCounter, mRenderCounter )
	
	//----------------------------------------------------------------//
					MOAIRenderMgr				();
					~MOAIRenderMgr				();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	void			Render						();
};

#endif