function TestOptionWindow()

    local width = Host.wWidth / 5.0
    local height = Host.wHeight / 4.0
    local posX = Host.wWidth / 3.0 - (width / 2.0)
    local posY = Host.wHeight / 3.0 - (height / 2.0)

    if Ctx:Begin("Select a test to run.", new_rect(posX, posY, width, height), PanelFlag.Title) then
        

        Ctx:End()
    end

end

return "TestOptionWindow", "Test Window Error!"
