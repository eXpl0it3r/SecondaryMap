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

local ip = "192.168.1.37";
local cached_url = "http://"..ip..":"..tostring(PORT).."/"..tostring(VERSION).."/";

local cycle_player = nil;
local sync_enabled = false;

--------------------------------------------------
-- Events
--------------------------------------------------

function on_component_load()
	-- Register slash commands
	LIB_SLASH.BindCallback({slash_list="/mapsync", description="Start syncing with Secondary Map Viewer", func=sync_toggle});
	
	cycle_player = Callback2.CreateCycle(send_player_data, nil);
end

function on_map_marker_added(args)
	if args.markerId ~= nil then
		local info = Game.GetMapMarkerInfo(args.markerId);
	
		log(tostring(info))
		
		local data = {};
		data["state"] = "add";
		data["id"] = info.id;
		data["type"] = info.markerType;
		data["x"] = info.x;
		data["y"] = info.y;
		--data["z"] = info.z;
		data["r"] = 255*info.icon_tint.r;
		data["g"] = 255*info.icon_tint.g;
		data["b"] = 255*info.icon_tint.b;
		data["a"] = 255*info.icon_tint.a;
		
		send_marker_data(data);
	end
end

function on_map_marker_removed(args)
	if args.markerId ~= nil then
		local data = {};
		data["state"] = "remove";
		data["id"] = args.markerId;
		
		send_marker_data(data);
	end
end

function on_map_marker_update(args)
	if args.markerId ~= nil then
		local info = Game.GetMapMarkerInfo(args.markerId);
		
		local data = {};
		data["state"] = "update";
		data["id"] = info.id;
		data["type"] = info.markerType;
		data["x"] = info.x;
		data["y"] = info.y;
		--data["z"] = info.z;
		data["r"] = 255*info.icon_tint.r;
		data["g"] = 255*info.icon_tint.g;
		data["b"] = 255*info.icon_tint.b;
		data["a"] = 255*info.icon_tint.a;
		
		send_marker_data(data);
	end
end

--------------------------------------------------
-- UI Helper Functions
--------------------------------------------------

function poi_markers()
	--[[local towerTable = {}
		local resourceList = {}
		local towerList = {}
		for idx,WO in pairs(Game.GetWorldObjectList()) do
			towerList[idx] = Game.GetWorldObjectInfo(WO)
			towerList.towerID = WO
			resourceList[idx] = Game.GetWorldObjectStatus(WO)
			resourceList[idx].towerId = WO
		end
		towerTable.instanceId = Chat.GetInstanceID()
		towerTable.TowerStatus = resourceList
		towerTable.TowerInfo = towerList
		towerTable.zoneId = Game.GetZoneId()
		towerTable.zoneName = Game.GetZoneInfo(Game.GetZoneId()).name
		HTTP.IssueRequest(KILLPOST, "POST", towerTable, EchoEcho)
		Callback2.FireAndForget(DumpTowers, nil, 120)

	local data {};
	data["state" = "add";
	data["id"] = ;
	data["type"] = info.markerType;
	data["x"] = info.x;
	data["y"] = info.y;
	--data["z"] = info.z;
	data["r"] = 255*info.icon_tint.r;
	data["g"] = 255*info.icon_tint.g;
	data["b"] = 255*info.icon_tint.b;
	data["a"] = 255*info.icon_tint.a;]]--
end

function clear_markers()
	local data = {};
	data["state"] = "clear";
	
	send_marker_data(data);
end

function sync_markers()
	local markers = Game.GetMapMarkerList();
	local data = {};
	data["state"] = "sync";
	data["markers"] = {};
	
	for _, m in pairs(markers) do
		local info = Game.GetMapMarkerInfo(m.markerId);
		
		local marker = {};
		marker["id"] = info.id;
		marker["type"] = info.markerType;
		marker["x"] = info.x;
		marker["y"] = info.y;
		--marker["z"] = info.z;
		marker["r"] = 255*info.icon_tint.r;
		marker["g"] = 255*info.icon_tint.g;
		marker["b"] = 255*info.icon_tint.b;
		marker["a"] = 255*info.icon_tint.a;
	
		data["markers"][#data["markers"]+1] = marker;
	end
	send_marker_data(data);
end

function sync_start()
	sync_enabled = true;
	
	cycle_player:Run(UPDATE_INTERVAL);
	--clear_markers();
	sync_markers();
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

function send_marker_data(data)
	issue_retry({suffix = "marker", content = data});
end

function send_player_data()
	local data = {};
	local player_pos = Player.GetPosition();
	local aim_pos = Player.GetAimPosition();
	
	data["x"] = player_pos.x;
	data["y"] = player_pos.y;
	data["rotation"] = -(math.atan2(player_pos.y - aim_pos.y, player_pos.x - aim_pos.x)+(math.pi/2));
	
	issue_fast({suffix = "player", content = data});
end

--------------------------------------------------
-- Networking
--------------------------------------------------

-- Issue an HTTP retry request
function issue_retry(args)
	local url = cached_url..args.suffix;

	if sync_enabled then
		if not HTTP.IsRequestPending(url) then
	
			log(tostring(args.content));
			HTTP.IssueRequest(url, "POST", args.content, on_send_completed)
		else
			callback(issue_retry, args, .5)
		end
	end
end

-- Issue an HTTP fast request
function issue_fast(args)
	local url = cached_url..args.suffix;

	if sync_enabled and not HTTP.IsRequestPending(url) then
		HTTP.IssueRequest(url, "POST", args.content, on_send_completed)
	end
end

-- Callback once an HTTP request has been finished
function on_send_completed(args, err)
    if err then
        local errorMessage = "request to ["..err.url.."] failed. Reason: "..err.message
        --Debug.Error(erorrMessage)
				log(errorMessage);
    end
end
