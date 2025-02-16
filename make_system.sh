read -r -d '' CONTEXT_PREFIX <<'EOF'
I now provide context on how you should respond to this prompt. You will receive a description of a system of bodies, your task is to generate this system of bodies in a format (that I describe below), so that it can be simulated as part of an n-body simulation.

I now describe the aforementioned format in which you are expected to respond to this prompt. Each body is described by mass, radius, position, velocity and acceleration. Mass and radius are floats, while position, velocity and acceleration are vectors composed of two floats. You can output a single body as follows:

<mass> <radius>
<x_position> <y_position>
<x_velocity> <y_velocity>
<x_acceleration> <y_acceleration>

Different body are separated by newlines:
<mass> <radius>
<x_position> <y_position>
<x_velocity> <y_velocity>
<x_acceleration> <y_acceleration>

<mass> <radius>
<x_position> <y_position>
<x_velocity> <y_velocity>
<x_acceleration> <y_acceleration>

...

You are also expected to keep the following guidelines in mind when generating this template:
1. A small body should have a radius of around 1, a big body would have a radius of around 10 and a very big body of around 20. Of course any value is permitted, this is just a tip for the simulation to look good.
2. Only bodies within the range -900 < x < 900 and -900 < y < 900 are visible. So you should try to place bodies within this range.
3. If the system description is vague you are allowed to fill the holes yourself. For example if the description is just "Two bodies" you are allowed to make up the properties of the bodies, do try to respect the guidelines and make the system interesting though.
4. Bodies should never ever overlap! This is the most important guideline and should be interpreted as strictly as possible.

I now give a practical example. If the system description is "Generate a system with two bodies, of same mass but different size. The bodies are on the same y horizontal" an valid output would be:

5 5
-100 0
0 0
0 0

5 10
100 0
0 0
0 0

Finally, the description may not always talk about 'bodies' but could ask you to generate the solar system for example, or "100 particles", you are to infer that in such cases we want the planets, or particles to be bodies in our system.

After this line is the system description:

EOF


ANTHROPIC_API_KEY="";

# Function to display usage
usage() {
    echo "Usage: $0 \"<description>\""
    echo "Example: $0 \"A system of 10 particles moving in a spiral pattern\""
    exit 1
}

# Check if description argument is provided
if [ ! $# -eq 1 ]; then
    usage
fi

DESCRIPTION=$1
PROMPT="${CONTEXT_PREFIX}${DESCRIPTION}"
echo $PROMPT

curl https://api.anthropic.com/v1/messages \
     --header "x-api-key: $ANTHROPIC_API_KEY" \
     --header "anthropic-version: 2023-06-01" \
     --header "content-type: application/json" \
     --data \
'{
    "model": "claude-3-5-sonnet-20241022",
    "max_tokens": 1024,
    "messages": [
        {"role": "user", "content": "Hello, world"}
    ]
}'
