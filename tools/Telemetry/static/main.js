const DEFAULT_PERIOD    = 1000;
const SUCCESS           = "SUCCESS";
const URL               = "http://localhost:5001";
//Better to construct options first and then pass it as a parameter
var serial              = "";
var telemetry_raw       = "";
var telemetry           = { };
var device_connected    = false;
var server_connected    = false;
var list                = [];
var serial_period       = DEFAULT_PERIOD;
var telemetry_period    = DEFAULT_PERIOD;
var telemetry_flag      = true;

var serial_output = $("#serial-output");
var telemetery_raw_field = $("#telemetry-raw");
var scrolled = false;

function setCookie(cname, cvalue, exdays)
{
    var d = new Date();
    d.setTime(d.getTime() + (exdays*24*60*60*1000));
    var expires = "expires="+ d.toUTCString();
    document.cookie = cname + "=" + cvalue + ";" + expires + ";path=/";
}

function getCookie(cname)
{
    var name = cname + "=";
    var decodedCookie = decodeURIComponent(document.cookie);
    var ca = decodedCookie.split(';');
    for(var i = 0; i <ca.length; i++)
    {
        var c = ca[i];
        while (c.charAt(0) == ' ')
        {
            c = c.substring(1);
        }
        if (c.indexOf(name) == 0)
        {
            return c.substring(name.length, c.length);
        }
    }
    return "";
}

function checkCookie()
{
    var user = getCookie("username");
    var result = false;
    if (user != "")
    {
        result = true;
    }
    return result;
}

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
    var val = $("#serial-frequency-select").val();
    var frequency = parseInt(val);
    serial_period = (frequency === -1) ? DEFAULT_PERIOD : 1000/frequency;
    setCookie("serial-frequency-select", val, 30);
});

$("#telemetry-frequency-select").on("change", () =>
{
    var val = $("#telemetry-frequency-select").val();
    var frequency = parseInt(val);
    telemetry_period = (frequency === -1) ? DEFAULT_PERIOD : 1000/frequency;
    setCookie("telemetry-frequency-select", val, 30);
});

$('#telemetry-on').on('change', function()
{
    telemetry_flag = $(this).is(":checked");
    setCookie("telemetry-on", telemetry_flag, 30);
});

$('#reset-on-connect').on('change', function()
{
    reset_on_connect_flag = $(this).is(":checked");
    setCookie("reset-on-connect", reset_on_connect_flag, 30);
});

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
    if(device_connected && server_connected && telemetry_flag)
    {
        $.get(`${URL}/telemetry`, function (data)
        {
            if(data !== telemetry_raw)
            {
                telemetry_raw = data;
                telemetery_raw_field.val(data);
                telemetry = parseTelemetry(data);
                if(!table_init)
                {
                    table_html = generateTable(telemetry);
                    $("#telemetry-table tbody").html(table_html);
                    table_init = true;
                }
                else
                {
                    updateTable();
                }
                //console.log();
                //updateScroll();
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
            $("#server-connection-indicator").removeClass("disconnected-text").addClass("connected-text");
            setTimeout(checkConnection, serial_period);
        },
        error: () =>
        {
            server_connected = false;
            $("#server-connection-indicator").removeClass("connected-text").addClass("disconnected-text");
            // alert("You are no longer connected to Telemetry Server.\nRestart Telemetry server and reload page.");
        }
    });
}

window.onload = function()
{
    setTimeout(function()
    {
        console.log("testing");
        checkConnection();
        getSerial();
        getTelemetry();
        $("#refresh").click();

        if(checkCookie('telemetry-on'))
        {
            $('#telemetry-on').prop('checked', getCookie('telemetry-on') === 'true');
        }
        if(checkCookie('reset-on-connect'))
        {
            $('#reset-on-connect').prop('checked', getCookie('reset-on-connect') === 'true');
        }
        if(checkCookie('serial-frequency-select'))
        {
            $("#serial-frequency-select").val(getCookie('serial-frequency-select'));
        }
        if(checkCookie('telemetry-frequency-select'))
        {
            $("#telemetry-frequency-select").val(getCookie('telemetry-frequency-select'));
        }
    }, 1000);
};