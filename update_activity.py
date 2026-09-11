import json
import datetime
import random
import pytz

# Configuration - Change timezone here
TIMEZONE = 'Asia/Karachi' 

def get_timezone_object(tz_name):
    """Get timezone object based on configuration"""
    timezone_map = {
        'Asia/Karachi': pytz.timezone('Asia/Karachi'),  
        'UTC': pytz.UTC
    }
    return timezone_map.get(tz_name, pytz.UTC)

def get_timezone_suffix(tz_name):
    """Get timezone suffix for display"""
    suffix_map = {
        'Asia/Karachi': 'PKT',
        'UTC': 'UTC'
    }
    return suffix_map.get(tz_name, 'UTC')

def update_activity():
    tz = get_timezone_object(TIMEZONE)
    tz_suffix = get_timezone_suffix(TIMEZONE)
    current_time = datetime.datetime.now(tz)
    
    formatted_time = current_time.strftime(f"%Y-%m-%d %H:%M:%S {tz_suffix}")
    current_date = current_time.strftime("%Y-%m-%d")
    
    # Read existing commits data
    try:
        with open('data/commits.json', 'r') as f:
            commits_data = json.load(f)
    except FileNotFoundError:
        commits_data = {"timezone": TIMEZONE, "commits": []}
    
    # Read existing streaks data
    try:
        with open('data/streaks.json', 'r') as f:
            streaks_data = json.load(f)
    except FileNotFoundError:
        streaks_data = {"timezone": TIMEZONE, "streak_count": 0, "streaks": []}
    
    # Add new commit entry
    commit_messages = [
        "Daily activity update 🚀 ",
        "Keeping the streak alive ⚡ ",
        "Another day, another commit 💪 ",
        "Daily contribution 📈 ",
        "Automated daily update 🤖 ",
        "Consistency is key 🔑 ",
        "Never miss a day 🎯 ",
        "Building habits one commit at a time 🏗️ ",
        "Daily dose of coding ☕ ",
        "Streak continues 🔥 "
    ]
    
    new_commit = {
        "timestamp": formatted_time,
        "date": current_date,
        "commit_number": len(commits_data["commits"]) + 1,
        "message": random.choice(commit_messages),
        "timezone": tz_suffix
    }
    
    commits_data["commits"].append(new_commit)
    commits_data["timezone"] = TIMEZONE  
    
    # Update streaks data
    streaks_data["streak_count"] += 1
    streak_entry = {
        "day": streaks_data["streak_count"],
        "timestamp": formatted_time,
        "date": current_date,
        "description": f"Day {streaks_data['streak_count']}: {formatted_time}"
    }
    
    streaks_data["streaks"].append(streak_entry)
    streaks_data["timezone"] = TIMEZONE 
    
    with open('data/commits.json', 'w') as f:
        json.dump(commits_data, f, indent=2)
    
    with open('data/streaks.json', 'w') as f:
        json.dump(streaks_data, f, indent=2)
    
    print(f"Activity updated at {formatted_time}")
    print(f"Total commits: {len(commits_data['commits'])}")
    print(f"Current streak: {streaks_data['streak_count']} days")

if __name__ == "__main__":
    update_activity()
