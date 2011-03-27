print ("CAN I HAZ LUABURGER?")

MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX, 1, 1, 1, 1, 1 )

layer = MOAILayer2D.new ()
layer:showDebugLines ()
MOAISim.pushRenderPass ( layer )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )
layer:setViewport ( viewport )

charcodes = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,?!"

font = MOAIFont.new ()
font:loadFromTTF ( "EBOLA-KI.ttf", charcodes, 16, 163 )

test = "a b c d e f g h i j k l m n o p q r s t u v w x y z A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"
test2 = "1 2 3 4 5 6 7 8 9 0 a i e o u sa shi se su so ma mi me mu mo ta chi te tsu to ka ki ke ku ko ra ri re ru ro"

textbox = MOAITextBox.new ()
textbox:setFont ( font )
textbox:setTextSize ( font:getScale ())
textbox:setRect ( -160, 0, 160, 200 )
textbox:setYFlip ( true )
layer:insertPrim ( textbox )

MOAISim.openWindow ( "textbox", 320, 480 )

function main ()

	-- start spooling the text
	textbox:setString ( test )
	textbox:spool ()
	
	-- more will be true as long as there is any remaining text
	while textbox:more () do
		
		-- wait until we're done spooling the page
		while textbox:isBusy () do coroutine.yield () end
		
		-- if there's more to spool, show the next page
		if textbox:more () then
		
			-- this shows the next page and restarts the spool action
			textbox:nextPage ()
			textbox:spool ()
		end
	end
	
	-- new text, new loop to test page resetting
	textbox:setString ( test2 )
	
	-- restart the spool action
	textbox:spool ()

	
	-- more will be true as long as there is any remaining text
	while textbox:more () do
		
		-- wait until we're done spooling the page
		while textbox:isBusy () do coroutine.yield () end
		
		-- if there's more to spool, show the next page
		if textbox:more () then
			
			-- this shows the next page and restarts the spool action
			textbox:nextPage ()
			textbox:spool ()
		end
	end
end

thread = MOAIThread.new ()
thread:run ( main )
