// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAITileDeck2D.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAITransformBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	setRect
	@text	Set the model space dimensions of a single tile. When grid drawing, this
			should be a unit rect centered at the origin for tiles that fit each grid
			cell. Growing or shrinking the rect will cause tiles to overlap or leave
			gaps between them.
	
	@in		MOAITileDeck2D self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAITileDeck2D::_setRect ( lua_State* L ) {
	LUA_SETUP ( MOAITileDeck2D, "UNNNN" )
	
	self->mRect = state.GetRect < float >( 2 );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSize
	@text	Controls how the texture is subdivided into tiles. Default
			behavior is to subdivide the texture into N by M tiles,
			but is tile dimensions are provided (in UV space) then the resulting
			tile set will be N * tileWidth by M * tileHeight in UV
			space. This means the tile set does not have to fill all of the
			texture. The upper left hand corner of the tile set will always be
			at UV 0, 0.
	
	@in		MOAITileDeck2D self
	@in		number width			Width of the tile deck in tiles.
	@in		number height			Height of the tile deck in tiles.
	@opt	number tileWidth		Width of individual tile in UV space. Defaults to 1 / width.
	@opt	number tileHeight		Height of individual tile in UV space. Defaults to 1 / height.
	@out	nil
*/
int	MOAITileDeck2D::_setSize ( lua_State* L ) {
	LUA_SETUP ( MOAITileDeck2D, "UNN" )
	
	u32 width = state.GetValue < u32 >( 2, 0 );
	u32 height = state.GetValue < u32 >( 3, 0 );
	float tileWidth = state.GetValue < float >( 4, 1.0f / ( float )width );
	float tileHeight = state.GetValue < float >( 5, 1.0f / ( float )height );
	
	self->SetWidth ( width );
	self->SetHeight ( height );
	self->SetTileWidth ( tileWidth );
	self->SetTileHeight ( tileHeight );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTexture
	@text	Set or load a texture for this deck.
	
	@in		MOAITileDeck2D self
	@in		variant texture		A MOAITexture, a MOAIDataBuffer or a path to a texture file
	@opt	number transform	Any bitwise combination of MOAITexture.QUANTIZE, MOAITexture.TRUECOLOR, MOAITexture.PREMULTIPLY_ALPHA
	@out	MOAITexture texture
*/
int	MOAITileDeck2D::_setTexture ( lua_State* L ) {
	LUA_SETUP ( MOAITileDeck2D, "U" )
	
	self->mTexture = MOAITexture::AffirmTexture ( state, 2 );
	if ( self->mTexture ) {
		self->mTexture->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//================================================================//
// MOAITileDeck2D
//================================================================//

//----------------------------------------------------------------//
bool MOAITileDeck2D::Bind () {

	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	if ( !drawBuffer.BindTexture ( this->mTexture )) return false;
	USGLQuad::BindVertexFormat ( drawBuffer );

	return true;
}

//----------------------------------------------------------------//
void MOAITileDeck2D::Draw ( u32 idx, float xOff, float yOff, float xScale, float yScale ) {
	
	idx = idx - 1;		
	
	USRect uvRect = this->GetTileRect ( idx );
	uvRect.FlipY ();
	
	USGLQuad quad;
	quad.SetVerts ( this->mRect );
	quad.SetUVs ( uvRect );
	quad.Draw ( xOff, yOff, xScale, yScale );
}

//----------------------------------------------------------------//
USRect MOAITileDeck2D::GetBounds ( u32 idx ) {
	UNUSED ( idx );

	return this->mRect;
}

//----------------------------------------------------------------//
MOAITileDeck2D::MOAITileDeck2D () {
	
	RTTI_SINGLE ( MOAIDeck2D )
	this->SetContentMask ( MOAIProp::CAN_DRAW );
	
	this->mRect.Init ( -0.5f, -0.5f, 0.5f, 0.5f );
}

//----------------------------------------------------------------//
MOAITileDeck2D::~MOAITileDeck2D () {
}

//----------------------------------------------------------------//
void MOAITileDeck2D::RegisterLuaClass ( USLuaState& state ) {

	this->MOAIDeck2D::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAITileDeck2D::RegisterLuaFuncs ( USLuaState& state ) {

	this->MOAIDeck2D::RegisterLuaFuncs ( state );

	LuaReg regTable [] = {
		{ "setRect",			_setRect },
		{ "setSize",			_setSize },
		{ "setTexture",			_setTexture },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITileDeck2D::SerializeIn ( USLuaState& state, USLuaSerializer& serializer ) {

	USGridSpace::SerializeIn ( state );
	
	this->mTexture = serializer.GetRefField < MOAITexture >( state, -1, "mTexture" );
}

//----------------------------------------------------------------//
void MOAITileDeck2D::SerializeOut ( USLuaState& state, USLuaSerializer& serializer ) {

	USGridSpace::SerializeOut ( state );
	
	serializer.SetRefField ( state, -1, "mTexture", this->mTexture );
}

//----------------------------------------------------------------//
STLString MOAITileDeck2D::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mTexture )

	return repr;
}