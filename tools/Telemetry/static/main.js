Highcharts.setOptions({
    plotOptions: {
        area: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        arearange: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        areaspline: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        // areasplinerange: { animation: false, enableMouseTracking: false, stickyTracking: true, shadow: false, dataLabels: { style: { textShadow: false } } },
        // bar: { animation: false, enableMouseTracking: false, stickyTracking: true, shadow: false, dataLabels: { style: { textShadow: false } } },
        boxplot: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        bubble: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        column: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        columnrange: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        errorbar: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        funnel: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        gauge: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        heatmap: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        line: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        pie: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        polygon: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        pyramid: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        scatter: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        series: {
            animation: false,
            enableMouseTracking: true,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        solidgauge: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        spline: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        treemap: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
        waterfall: {
            animation: false,
            enableMouseTracking: false,
            stickyTracking: true,
            shadow: false,
            dataLabels: {
                style: {
                    textShadow: false
                }
            }
        },
    },
    chart: {
        reflow: false,
        events: {
            redraw: function() {
                // console.log("highcharts redraw, rendering-done");
                // $('body').addClass('rendering-done');
            }
        },
        animation: false
    },
    tooltip: {
        enabled: true,
        animation: false
    },
    exporting: {
        enabled: false
    },
    credits: {
        enabled: false
    }
});
const GRAPHING_OPTIONS = {
    rangeSelector: {
        buttons: [{
            count: 10,
            type: 'second',
            text: '10s'
        }, {
            count: 20,
            type: 'second',
            text: '20s'
        }, {
            count: 30,
            type: 'second',
            text: '30s'
        }, {
            type: 'all',
            text: 'All'
        }],
        inputEnabled: false,
        selected: 0
    },

    title: {
        text: 'Live random data'
    },

    exporting: {
        enabled: true
    },

    series: [{
        name: 'Random data',
        data: (function() {
            // generate first set of data
            var data = [];
            var time = (new Date()).getTime();
            for (var i = -10; i <= 0; i += 1)
            {
                data.push([
                    time + i * 1000,
                    0
                ]);
            }
            return data;
        }())
    }]
};

const DEFAULT_PERIOD    = 1000;
const SUCCESS           = "SUCCESS";
const URL               = "http://localhost:5001";
//Better to construct options first and then pass it as a parameter
var serial              = "";
var telemetry_raw       = "";
var telemetry           = { };
var device_connected    = false;
var server_connected    = false;
var list                = [ ];
var graph_options       = { };
var graphs              = { };
var graph_update_active = true;
var redraw_counter      = 0;
var graph_telem_update_ratio = 1;
var serial_period       = DEFAULT_PERIOD;
var telemetry_period    = DEFAULT_PERIOD;
var graph_period        = DEFAULT_PERIOD;
var telemetry_flag      = true;
var table_init          = false;
var carriage_return_active = false;
var newline_select      = true;

var serial_output       = $("#serial-output");
var telemetery_raw_field = $("#telemetry-raw");
var scrolled_to_bottom  = true;

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

function checkCookie(cookie)
{
    var user = getCookie(cookie);
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
                $("#serial-baud-select").attr("disabled", "disabled");
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
                table_init = false;
                telemetry_raw = "\r\n";
                $("#connect")
                    .addClass("btn-outline-success")
                    .removeClass("btn-outline-danger")
                    .text("Connect");
                $("#serial-baud-select").removeAttr("disabled");
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
        $("input[name='serial-input']").val("");

        var cr = (carriage_return_active) ? "1" : "0";
        var nl = (newline_select) ? "1" : "0";

        $.get(`${URL}/write/${payload}/${cr}/${nl}`, function( data )
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

    if(telemetry_period > graph_period)
    {
        $("#graph-frequency-select").val(val);
    }

    $("#graph-frequency-select option").filter(function() {
        return $(this).attr("value") > $("#telemetry-frequency-select").val();
    }).attr("disabled", "disabled");

    $("#graph-frequency-select option").filter(function() {
        return $(this).attr("value") <= $("#telemetry-frequency-select").val();
    }).removeAttr("disabled");
});

$("#serial-baud-select").on("change", () =>
{
    var val = $("#serial-baud-select").val();
    $.get(`${URL}/baudrate/${val}`, function( data )
    {
        if(data === SUCCESS) {}
    });
    setCookie("serial-baud-select", val, 30);
});

$("#graph-frequency-select").on("change", () =>
{
    var val = $("#graph-frequency-select").val();
    var frequency = parseInt(val);
    graph_period = (frequency === -1) ? DEFAULT_PERIOD : 1000/frequency;
    setCookie("graph-frequency-select", val, 30);
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

$('#graph-switch').on('change', function()
{
    graph_update_active = $(this).is(":checked");
    setCookie("graph-switch", graph_update_active, 30);
});

$('#carriage-return-select').on('change', function()
{
    carriage_return_active = $(this).is(":checked");
    setCookie("carriage-return-active", carriage_return_active, 30);
});

$('#newline-select').on('change', function()
{
    newline_select = $(this).is(":checked");
    setCookie("newline-select", newline_select, 30);
});

function updateScroll()
{
    if(scrolled_to_bottom)
    {
        var element = document.getElementById("serial-output");
        element.scrollTop = element.scrollHeight;
    }
}
serial_output.on('scroll', function()
{
    const SCROLLBAR_OFFSET = 20; // 20px
    var element = $("#serial-output");
    var height = element.height();
    var scrollTop = element.scrollTop();
    var scrollHeight = element[0].scrollHeight;

    var scroll_distance = SCROLLBAR_OFFSET+scrollTop+height;
    if(scrollHeight <= scroll_distance)
    {
        scrolled_to_bottom = true;
    }
    else
    {
        scrolled_to_bottom = false;
    }
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

function generateGraph()
{
    var html = "";
    graph_options = { };

    for(bucket in telemetry)
    {
        for(element in telemetry[bucket])
        {
            title = `${bucket}-${element}`;
            html += `
            <div class="col-lg-4">
                <div id="graph-${title}" style="height: 300px;"></div>
            </div>`;
            var struct = {};
            jQuery.extend(true, struct, GRAPHING_OPTIONS);
            struct.title.text = title;
            struct.series[0].name = element;
            graph_options[`graph-${title}`] = struct;
        }
    }
    return html;
}

function intializeGraphs()
{
    graph = { };
    for(graph in graph_options)
    {
        graphs[graph] = Highcharts.stockChart(graph, graph_options[graph])
    }
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

function updateGraph()
{
    for(bucket in telemetry)
    {
        for(element in telemetry[bucket])
        {
            var select = `graph-${bucket}-${element}`;
            var x = (new Date()).getTime();
            var y = parseFloat(telemetry[bucket][element]["value"]);
            try
            {
                graphs[select].series[0].addPoint([x, y], false, false);
            }
            catch(e)
            {
                return false;
            }
        }
    }
    return true;
}

function getTelemetry()
{
    if(device_connected && server_connected && telemetry_flag)
    {
        $.get(`${URL}/telemetry`, function (data)
        {
            if(data === "\r\n" || data === "")
            {
                console.log("rejecting");
                return;
            }
            graph_telem_update_ratio = (graph_period / telemetry_period);
            if(data !== telemetry_raw)
            {
                telemetry_raw = data;
                telemetery_raw_field.val(data);
                telemetry = parseTelemetry(data);
                if(!table_init)
                {
                    console.log("Initialize telemetry feedback", telemetry);
                    var table_html = generateTable(telemetry);
                    $("#telemetry-table tbody").html(table_html);
                    var graph_html = generateGraph(telemetry);
                    $("#graph-holder").html(graph_html);
                    intializeGraphs();
                    table_init = true;
                }
                else
                {
                    updateTable();
                    if(graph_update_active)
                    {
                        updateGraph();
                        if(redraw_counter >= graph_telem_update_ratio)
                        {
                            for(graph in graphs)
                            {
                                graphs[graph].redraw();
                            }
                            redraw_counter = 0;
                        }
                        redraw_counter++;
                    }
                }
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
            $('#myModal').modal('show');
            // setTimeout(checkConnection, serial_period);
        }
    });
}

window.onload = function()
{
    setTimeout(function()
    {
        checkConnection();
        getSerial();
        getTelemetry();
        $("#refresh").click();

        if(checkCookie('telemetry-on'))
        {
            $("#telemetry-on").prop("checked", getCookie("telemetry-on") === "true");
            $("#telemetry-on").change();
        }
        if(checkCookie("reset-on-connect"))
        {
            $("#reset-on-connect").prop("checked", getCookie("reset-on-connect") === "true");
            $("#reset-on-connect").change();
        }
        if(checkCookie("serial-frequency-select"))
        {
            $("#serial-frequency-select").val(getCookie("serial-frequency-select"));
            $("#serial-frequency-select").change();
        }
        if(checkCookie("telemetry-frequency-select"))
        {
            $("#telemetry-frequency-select").val(getCookie("telemetry-frequency-select"));
            $("#telemetry-frequency-select").change();
        }
        if(checkCookie("graph-switch"))
        {
            $("#graph-switch").prop("checked", getCookie("graph-switch") === "true");
            $("#graph-switch").change();
        }
        if(checkCookie("graph-frequency-select"))
        {
            $("#graph-frequency-select").val(getCookie("graph-frequency-select"));
            $("#graph-frequency-select").change();
        }
        if(checkCookie("serial-baud-select"))
        {
            $("#serial-baud-select").val(getCookie("serial-baud-select"));
            $("#serial-baud-select").change();
        }
        if(checkCookie("carriage-return-select"))
        {
            $("#carriage-return-select").val(getCookie("carriage-return-select"));
            $("#carriage-return-select").change();
        }
        if(checkCookie("newline-select"))
        {
            $("#newline-select").val(getCookie("newline-select"));
            $("#newline-select").change();
        }
    }, 100);
};