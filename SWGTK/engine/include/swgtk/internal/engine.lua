-- We are running the main loop from within Lua itself.
function main()
    if OnCreate() then
        while App:IsAppRunning() do
            App:EventsAndTimeStep()
            if not OnUpdate(App.Timer:GetSeconds()) then App:CloseApp() end
            Render:BufferPresent()
        end
    end
end