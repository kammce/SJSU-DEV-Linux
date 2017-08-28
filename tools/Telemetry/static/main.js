const DEFAULT_PERIOD 	= 1000;
const SUCCESS 			= "SUCCESS";
const URL 				= "http://127.0.0.1:5000";
var serial 				= "";
var telemetry_raw 		= "";
var telemetry 			= { };
var device_connected 	= false;
var server_connected 	= false;
var list 				= [];
var serial_period 		= DEFAULT_PERIOD;
var telemetry_period 	= DEFAULT_PERIOD;

function generateDropDownList(new_list)
{
	var html = `<option value="-1">Select Serial Device ...</option>`;
	for(var i = 0; i < new_list.length; i++)
	{
		item_number = new_list[i].replace("/dev/ttyUSB", "");
		html += `
			<option
				value="${item_number}"
				${(i === 0) ? "selected" : ""}>
				/dev/ttyUSB${item_number}
			</option>`;
	}
	return html;
}

$("#refresh").on("click", () =>
{
	$.get(`${URL}/list`, function( data )
	{
		var new_list = [];
		if(data)
		{
			try
			{
				new_list = JSON.parse(data);
				console.log(new_list);
			} catch(e) { }
			var list_html = generateDropDownList(new_list);
			$("#device-select").html(list_html);
		}
	});
});

$("#connect").on("click", () =>
{
	if(!device_connected)
	{
		var device_number = parseInt($("#device-select").val());
		var dev_str = (device_number === -1) ? '' : `/${device_number}`;
		$.get(`${URL}/connect${dev_str}`, function( data )
		{
			if(data === SUCCESS)
			{
				device_connected = true;
				$("#connect")
					.removeClass("btn-outline-success")
					.addClass("btn-outline-danger")
					.text("Disconnect");
			}
		});
	}
	else
	{
		$.get(`${URL}/disconnect`, function( data )
		{
			if(data === SUCCESS)
			{
				device_connected = false;
				$("#connect")
					.addClass("btn-outline-success")
					.removeClass("btn-outline-danger")
					.text("Connect");
			}
		});
	}
});

$("input[name='serial-input']").on('keyup', (e) =>
{
	if(e.keyCode === 13)
	{
		$("#serial-send").click();
	}
});

$("#serial-send").on("click", () =>
{
	if(device_connected)
	{
		var payload = $("input[name='serial-input']").val();

		$.get(`${URL}/write/${payload}`, function( data )
		{
			if(data === SUCCESS)
			{
				console.info("WRITE SUCCESS!");
			}
			else
			{
				console.info("WRITE FAILURE!");
			}
		});
	}
});

$("#serial-frequency-select").on("change", () =>
{
	var frequency = parseInt($("#serial-frequency-select").val());
	serial_period = (frequency === -1) ? DEFAULT_PERIOD : 1000/frequency;
});

$("#telemetry-frequency-select").on("change", () =>
{
	var frequency = parseInt($("#telemetry-frequency-select").val());
	telemetry_period = (frequency === -1) ? DEFAULT_PERIOD : 1000/frequency;
});

var serial_output = $("#serial-output");
var telemetery_raw_field = $("#telemetry-raw");
var scrolled = false;

function updateScroll()
{
    if(!scrolled)
    {
        var element = document.getElementById("serial-output");
        element.scrollTop = element.scrollHeight;
    }
}

serial_output.on('scroll', function()
{
    //scrolled = true;
});

function getSerial()
{
	if(device_connected && server_connected)
	{
		$.get(`${URL}/serial`, function( data )
		{
			if(data !== serial)
			{
				serial = data;
				serial_output.val(serial);
				updateScroll();
			}
		});
	}
	setTimeout(getSerial, serial_period);
}

function parseTelemetry()
{
	var json = {};

	buckets = telemetry_raw.split("START:");
	for(bucket of buckets)
	{
		components = bucket.split("\n");
		var current_bucket = components[0].split(":")[0];
		json[current_bucket] = { };

		for(component of components)
		{
			sections = component.split(":");
			switch(sections.length)
			{
				case 0:
					break;
				case 2:
					break;
				case 6:
					json[current_bucket][sections[0]] = {
						type: sections[4],
						value: sections[5]
					};
					break;
				default:
					break;
			}
		}
	}
	return json;
}

function telemetrySet(bucket, element)
{
	var new_value = $(`input[name="set-${bucket}-${element}"]`).val();
	$.get(`${URL}/set/${bucket}/${element}/${new_value}`, function( data )
	{
		if(data === SUCCESS) {}
	});
}

var table_init = false;

function generateTable()
{
	var html = "";

	for(bucket in telemetry)
	{
		for(element in telemetry[bucket])
		{
			html += `
			<tr id="${bucket}-${element}">
				<td>${bucket}</td>
				<td>${element}</td>
				<td>${telemetry[bucket][element]["type"]}</td>
				<td>${telemetry[bucket][element]["value"]}</td>
				<td>
					<input class="form-control" size="1" type="text" name="set-${bucket}-${element}"/>
				</td>
				<td>
					 <button
					 	class="btn btn-outline-success my-2 my-sm-0"
					 	id="set-${bucket}-${element}-btn"
					 	type="submit"
					 	onclick="telemetrySet('${bucket}', '${element}')">Set</button>
				</td>
			</tr>`;
		}
	}
	return html;
}

function updateTable()
{
	for(bucket in telemetry)
	{
		for(element in telemetry[bucket])
		{
			var select = `#${bucket}-${element} td`;
			var cell = $(select)[3];
			$(cell).html(telemetry[bucket][element]["value"]);
		}
	}
}

function getTelemetry()
{
	if(device_connected && server_connected)
	{
		$.get(`${URL}/telemetry`, function (data)
		{
			if(data !== telemetry_raw)
			{
				telemetry_raw = data;
				telemetery_raw_field.val(data);
				telemetry = parseTelemetry(data);
				html = generateTable(telemetry);
				if(!table_init)
				{
					$("#telemetry-table tbody").html(html);
					table_init = true;
				}
				else
				{
					updateTable();
				}
				//console.log();
				// updateScroll();
			}
		});
	}
	setTimeout(getTelemetry, telemetry_period);
}

function checkConnection()
{
	$.ajax({
		url: `${URL}/server-is-alive`,
		type: 'GET',
		success: () =>
		{
			server_connected = true;
			setTimeout(checkConnection, serial_period);
		},
		error: () =>
		{
			server_connected = false;
			alert("You are no longer connected to Telemetry Server");
		}
	});
}

window.onload = function()
{
	console.log("testing");
	checkConnection();
	getSerial();
	getTelemetry();
	$("#refresh").click();
};


$(function () {
	//Better to construct options first and then pass it as a parameter
	var options = {
		title: {
			text: "Spline Chart using jQuery Plugin"
		},
                animationEnabled: true,
		data: [
		{
			type: "spline", //change it to line, area, column, pie, etc
			dataPoints: [
				{ x: 10, y: 10 },
				{ x: 20, y: 12 },
				{ x: 30, y: 8 },
				{ x: 40, y: 14 },
				{ x: 50, y: 6 },
				{ x: 60, y: 24 },
				{ x: 70, y: -4 },
				{ x: 80, y: 10 }
			]
		}
		]
	};

	$("#chartContainer").CanvasJSChart(options);

});