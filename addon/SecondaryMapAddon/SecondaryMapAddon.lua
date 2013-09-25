--------------------------------------------------
-- Copyright 2013 (C) DarkCisum
--------------------------------------------------

require "string"
require "math"
require "lib/lib_Callback2"
require "lib/lib_Debug"
require "lib/lib_Slash"

-- Addon version
local VERSION = 0.1
local PORT = 3344;
local UPDATE_INTERVAL = 2;

local ip = "192.168.1.200";
local cached_url = "http://"..ip..":"..tostring(PORT).."/"..tostring(VERSION).."/";

local cycle_player = nil;
local sync_enabled = false;

--------------------------------------------------
-- Events
--------------------------------------------------

function on_component_load()
	-- Register slash commands
	LIB_SLASH.BindCallback({slash_list="/mapsync", description="Start syncing with Secondary Map Viewer", func=sync_toggle})
	
	cycle_player = Callback2.CreateCycle(send_player_data, nil);
end

function on_map_marker_added(args)
	--log(tostring(args));
	log(tostring(Game.GetMapMarkerInfo(args.markerId)));
end

function on_map_marker_removed(args)
	--log(tostring(args));
	log(tostring(Game.GetMapMarkerInfo(args.markerId)));
end

function on_map_marker_update(args)
	--log(tostring(args));
	log(tostring(Game.GetMapMarkerInfo(args.markerId)));
end

--------------------------------------------------
-- UI Helper Functions
--------------------------------------------------

function sync_start()
	sync_enabled = true;
	
	cycle_player:Run(UPDATE_INTERVAL);
end

function sync_stop()
	sync_enabled = false;
	
	cycle_player:Stop();
end

function sync_toggle()
	if sync_enabled then
		sync_stop()
	else
		sync_start()
	end
end

--------------------------------------------------
-- Send Data
--------------------------------------------------

function send_player_data()
		local data = {};
		local player_pos = Player.GetPosition();
		local aim_pos = Player.GetAimPosition();
		
		data["x"] = player_pos.x;
		data["y"] = player_pos.y;
		data["rotation"] = -(math.atan2(player_pos.y - aim_pos.y, player_pos.x - aim_pos.x)+(math.pi/2));
		
		issue_data("player", data);
end

--------------------------------------------------
-- Networking
--------------------------------------------------

-- Issue an HTTP request
function issue_data(suffix, data)
    local url = cached_url..suffix;

    if not HTTP.IsRequestPending(url) then
        HTTP.IssueRequest(url, "POST", data, on_send_completed)
    end
end

-- Callback once an HTTP request has been finished
function on_send_completed(args, err)
    if err then
        local erorrMessage = "request to ["..err.url.."] failed. Reason: "..err.message
        Debug.Error(erorrMessage)
    end
end
