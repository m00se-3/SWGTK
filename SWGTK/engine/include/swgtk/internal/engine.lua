-- We are running the main loop from within Lua itself.
function main()
    if OnCreate() then
        while swgtk.App:IsAppRunning() do
            swgtk.App:EventsAndTimeStep()
            if not OnUpdate(swgtk.App.DeltaTime:GetSeconds()) then swgtk.App:CloseApp() end
            swgtk.Render:BufferPresent()
        end
    end
end