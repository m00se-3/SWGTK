-- This is supposed to be the file that defines the TestSuite options, assuming the UI code still works.

function TestOptionWindow()

    if Ctx:Begin("This worked.", new_rect(100.0, 100.0, 200.0, 100.0), PanelFlag.Title) then
        

        Ctx:End()
    end

end

return "TestOptionWindow", "Test Window Error!"
