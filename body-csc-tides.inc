    <!--prettier-ignore-->
    #errorCatcher Echo
    #encoding UTF-8
    #from datetime import datetime, timedelta

    #set $diagrams_config = $DisplayOptions.get("diagrams", {})
    #set global $layout = 'alternative'
    #set $gauges_display = $DisplayOptions.get('gauges_display', 'before')
    #set $show_gauges = 'Gauges' in $DisplayOptions and $context in $DisplayOptions['Gauges']


    <link rel="stylesheet" href="https://cdn.aerisapi.com/wxblox/latest/aeris-wxblox.css">
    <script src="https://cdn.aerisapi.com/wxblox/latest/aeris-wxblox.js"></script>

    <style>
    h3{
font-size:1.75rem;
    }
    .advisories{
    margin-top:0px!important;
    padding-top:10px!important;
    background-color: rgb(254, 215, 215)!important;
    border:solid 1px rgb(197, 31, 31)!important;
    color:hsl(116,0%,20%)!important;
    }
    .advisories a{
    color:hsl(116,0%,20%)!important;
    }
    </style> 

    <div id="wxblox" class="aeris-wrapper"></div>

    <script>    
    const aeris = new AerisWeather('6mHXhibAPUlZfTTiuE5r1', 'FJsUv4NWSB2DcjaJARF23g6LNuEBWAcBezqh9bt0');
    aeris.on('ready', () => {
        var view = new aeris.wxblox.views.Advisories('#wxblox', {
        fullDetails: false
    });
    view.load({
        p: "37.86,-122.31"
    });
    });
    </script>

    <style>
    .gauge-label
    {font-family:"IBM Plex Sans"!important;
    }
    .gauge-text text{font-family:"IBM Plex Sans"!important;}

    @media only screen and (min-device-width : 320px) and (max-device-width : 480px)  { 
    .gauge-text{font-size:70%;font-weight:300!important;font-family:"IBM Plex Sans"!important;}
    .gauge-text text{font-family:"IBM Plex Sans"!important;}
    .gauge-label
    {font-family:"IBM Plex Sans"!important;
    }
    }
    </style>

    <div class="bx--row">
      <div class="bx--col">
        <div class="bx--row">
    <div class="bx--col-sm-4 bx--col-md-8 bx--col-lg-12 bx--col-xlg-6 bx--col-max-8">




    #def render_gauges($gauge_config)
      #set $gauges_index = 0
      #for $gauge_obs in $gauge_config
        #set $gauge_obs_data_binding = $get_data_binding($gauge_obs)

        ## I dont know why but $alltime($data_binding=$skin_obs_binding) crashes on the month-%Y-%m and year-%Y pages:
        ## ValueError: start time (1654034700) is greater than stop time (1640991600)
        #set $alltime_tag = None if $use_parameter_year or $use_parameter_month else $alltime($data_binding=$gauge_obs_data_binding)
        #set global $gauge_obs_binding = $getattr($get_time_span_from_context($context, $day($data_binding=$gauge_obs_data_binding), $week($data_binding=$gauge_obs_data_binding), $month($data_binding=$gauge_obs_data_binding), $year($data_binding=$gauge_obs_data_binding), $alltime_tag, $yesterday($data_binding=$gauge_obs_data_binding)), $get_custom_data_binding_obs_key($gauge_obs))

        #if $gauge_obs_binding.has_data
          #set global $gauges_index = $gauges_index
          #set global $partial_obs = $gauge_obs
          #include "includes/gauge-tile.inc"
          #set $gauges_index = $gauges_index + 1
        #end if
      #end for
    #end def

    #set $stat_tiles_xaggs_same_row = []
    #set $stat_tiles_xaggs_extra_row = []
    #if 'stat_tiles_xaggs' in $DisplayOptions
      #set $stat_tiles_xaggs = $DisplayOptions.stat_tiles_xaggs

      #for $xaggs in $stat_tiles_xaggs
        #if $context != "day" and ('avg_context' not in $stat_tiles_xaggs[$xaggs] or $context not in $stat_tiles_xaggs[$xaggs]['avg_context'])
          #continue
        #end if

        #set $xaggs_to_append = {'obs': $xaggs, **$stat_tiles_xaggs[$xaggs]}

        #if $context == "day"
          #silent $xaggs_to_append['aggregate_types'] = $list(filter(lambda aggregate_type: 'historical' in aggregate_type, $xaggs_to_append['aggregate_types']))
        #else
          #silent $xaggs_to_append['aggregate_types'] = $list(filter(lambda aggregate_type: 'historical' not in aggregate_type, $xaggs_to_append['aggregate_types']))
        #end if

        #if $to_bool($stat_tiles_xaggs[$xaggs]['show_extra_row']) is True
          #silent $stat_tiles_xaggs_extra_row.append($xaggs_to_append)
        #else
          #silent $stat_tiles_xaggs_same_row.append($xaggs_to_append)
        #end if
      #end for
    #end if





    <!--prettier-ignore-->
    #if $show_gauges and $gauges_display == 'before'
    <!--prettier-ignore-->
    <div class="bx--row">
    $render_gauges($DisplayOptions['Gauges'][$context])
    </div>
    #end if

    <div class="bx--row">

      <!--prettier-ignore-->
      #for $skin_obs in $DisplayOptions.get('stat_tile_observations')
        #if $skin_obs == 'windSpeed' or $skin_obs == 'windGust'
          #set $skin_obs_binding = $get_data_binding($skin_obs)

          #set $show_sum = $get_show_sum($skin_obs)
          #set $show_min = $get_show_min($skin_obs)
          #set $show_max = $get_show_max($skin_obs)
          #set $use_day = $show_sum or $show_min or $show_max

          ## I dont know why but $alltime($data_binding=$skin_obs_binding) crashes on the month-%Y-%m and year-%Y pages:
          ## ValueError: start time (1654034700) is greater than stop time (1640991600)
          #set $alltime_tag = $alltime($data_binding=$skin_obs_binding) if $context == 'alltime' else None

          #if $context == 'day' and not $daily_archive and $use_day
            #set $stat_tile_check = $day
          #elif $context == 'day' and not $daily_archive and not $use_day
            #set $stat_tile_check = $current
          #else
            #set $stat_tile_check = $get_time_span_from_context($context, $day($data_binding=$skin_obs_binding), $week($data_binding=$skin_obs_binding), $month($data_binding=$skin_obs_binding), $year($data_binding=$skin_obs_binding), $alltime_tag, $yesterday($data_binding=$skin_obs_binding))
          #end if

          #if $getattr($stat_tile_check, $get_custom_data_binding_obs_key($skin_obs)).has_data
            #set global $partial_obs = $skin_obs
           #if $context != "day"
            #include "includes/stat-tile.inc"
           #end if


          #end if
        #end if
      #end for

      <!--prettier-ignore-->
      #if $stat_tiles_xaggs_same_row
        #set global $hide_xaggs_heading = False
        #for $stat_xaggs in $stat_tiles_xaggs_same_row
          #set global $stat_xaggs = $stat_xaggs
          #include "includes/stat-tile-xaggs.inc"
        #end for
      #end if
    </div>


    <!--prettier-ignore-->
    #if $show_gauges and $gauges_display == 'after'
    <!--prettier-ignore-->
    <div class="bx--row">
    $render_gauges($DisplayOptions['Gauges'][$context])
    </div>
    #end if

    <!--prettier-ignore-->
    #if $stat_tiles_xaggs_extra_row
    #set global $hide_xaggs_heading = True
    <!--prettier-ignore-->
    <header class="bx--col section-header">
      #if $context == 'day'
      <h2>$gettext("Historical values for today")</h2>
      #else
      <h2>$gettext("Historical values")</h2>
      #end if
    </header>
    <!--prettier-ignore-->
    <div class="bx--row">
      #for $stat_xaggs in $stat_tiles_xaggs_extra_row
        #set global $stat_xaggs = $stat_xaggs
        #include "includes/stat-tile-xaggs.inc"
      #end for
    </div>
    #end if


    <!--prettier-ignore-->
    <div class="bx--row">
      #include "includes/section-heading.inc"
    </div>


    <!-- prettier-ignore -->

    <div class="bx--row">
    #set $diagram_index = 0
    #set $context_for_diagrams = $context if $context != 'yesterday' else 'day'
    #set $diagram_context_config = $DisplayOptions.get("diagrams", {}).get($context_for_diagrams, {}).get('observations', {})
    #if "combined_observations" in $diagrams_config
      #set $combined = $diagrams_config.combined_observations
    #else
      #set $combined = {}
    #end if

    #for $skin_obs in $diagram_context_config
      #if $skin_obs in $combined.keys()
        #set $combined_diagram = $combined[$skin_obs]
        #for $combined_obs in $combined_diagram.obs.keys()
          #set $skin_obs_binding = $get_data_binding_combined_diagram($combined_obs, $combined_diagram, $skin_obs, $context)

          ## I dont know why but $alltime($data_binding=$skin_obs_binding) crashes on the month-%Y-%m and year-%Y pages:
          ## ValueError: start time (1654034700) is greater than stop time (1640991600)
          #set $alltime_tag = $alltime($data_binding=$skin_obs_binding) if $context == 'alltime' else None

          #if $getattr($get_time_span_from_context($context, $day($data_binding=$skin_obs_binding), $week($data_binding=$skin_obs_binding), $month($data_binding=$skin_obs_binding), $year($data_binding=$skin_obs_binding), $alltime_tag, $yesterday($data_binding=$skin_obs_binding)), $get_custom_data_binding_obs_key($combined_diagram.obs[$combined_obs].observation)).has_data or $combined_diagram.obs[$combined_obs].observation in $get_software_obs
            #set global $comb_obs = $skin_obs
            #set global $combined_diagram = $combined_diagram
            #set global $diagram_index = $diagram_index
            #include "includes/combined-diagram-tile.inc"
            #set $diagram_index = $diagram_index + 1
            #break
          #end if
        #end for
      #else
        #set $skin_obs_binding = $get_data_binding($skin_obs, $context)
        #set $alltime_tag = $alltime($data_binding=$skin_obs_binding) if $context == 'alltime' else None
        #if $getattr($get_time_span_from_context($context, $day($data_binding=$skin_obs_binding), $week($data_binding=$skin_obs_binding), $month($data_binding=$skin_obs_binding), $year($data_binding=$skin_obs_binding), $alltime_tag, $yesterday($data_binding=$skin_obs_binding)), $get_custom_data_binding_obs_key($skin_obs)).has_data or (($skin_obs == 'windDir' or $skin_obs == 'windRose') and $getattr($get_time_span_from_context($context, $day, $week, $month, $year, $alltime, $yesterday), 'windSpeed').has_data or $skin_obs in $get_software_obs)
          #set global $partial_obs = $skin_obs
          #set global $diagram_index = $diagram_index
          #if $skin_obs == 'windRose'
            #include "includes/diagram-tile-wind-rose.inc"
          #else
            #include "includes/diagram-tile.inc"
          #end if
          #set $diagram_index = $diagram_index + 1
        #end if
      #end if
    #end for



    </div>

    <!--prettier-ignore-->
    #if $context == "day" and $daily_archive == False
    #include "includes/forecast.inc"
    #end if
    </div>




    #set $warn_region_front = $Extras['weewx-DWD']['show_warnings_on_front'] if 'weewx-DWD' in $Extras and 'show_warnings_on_front' in $Extras['weewx-DWD'] else False
    #set $show_warn = True if $warn_region_front and $dwd_warning_has_warning($warn_region_front) else False


    #set $externals_list = []
    #if 'externals' in $Extras and $len($Extras['externals']) > 0
      #for $external in $Extras['externals']
        #if $external == 'description'
          #continue
        #end if
        #if $to_bool($Extras['externals'][$external].show_on_front)
          #silent $externals_list.append($external)
        #end if
      #end for
    #end if

    #set $radar_externals = 'radar_html' in $Extras or $len($externals_list) > 0

    #set $radar_externals_list = $externals_list
    #if 'radar_html' in $Extras
      #set $radar_externals_list = ['radar'] + $externals_list
    #end if


    <!--prettier-ignore-->
          #if $radar_externals or $show_warn
          #set radar_classes = "bx--col-sm-4 bx--col-md-8 bx--col-lg-12 bx--col-xlg-6 bx--col-max-4"
          #if 'forecast_radar_equal_width' in $Extras and $to_bool($Extras.forecast_radar_equal_width) and $DisplayOptions.get('layout', 'alternative') == 'alternative'
            #set radar_classes = "bx--col-sm-4 bx--col-md-8 bx--col-lg-12 bx--col-xlg-6 bx--col-max-6"
          #end if
          #if $DisplayOptions.get('layout', 'alternative') == 'classic'
            #set radar_classes = "bx--col-sm-4 bx--col-md-12 bx--col-lg-12 bx--col-xlg-12 bx--col-max-12"
          #end if

          <!--prettier-ignore-->
          <div class="$radar_classes radar-container">
            #if $show_warn
              #set global $region = $get_dwd_warning_region_name($warn_region_front)
              #set global $region_key = $warn_region_front
              #include "includes/dwd-warning.inc"
            #end if

            #if $radar_externals

            <style>
              .radar-container .bx-ce-demo-devenv--tab-panels > div {
                padding-bottom: 1rem;
              }
              .radar-container .bx-ce-demo-devenv--tab-panels > div > .bx--tile {
                padding: 0;
              }
            </style>

            #if len($radar_externals_list) == 1
              <style>
                .radar-container .bx-ce-demo-devenv--tab-panels .bx--tile {
                  padding: 0;
                }
              </style>
            #end if




        <h3><strong>Club Tides &amp; Hours</strong></h3><br/>



        <div class="bx--tile tile-radar-forecast">


            <script src="https://cdn.tailwindcss.com"></script>
        <script src="https://d3js.org/d3.v7.min.js"></script>
        
        <style>
            .tide-chart-container {
                touch-action: manipulation;
            }
            .axis path,
            .axis line {
                stroke: #d1d5db; /* Lighter axis lines */
            }
            .axis text {
                fill: #6b7280; /* Darker text for readability */
                font-size: 0.75rem;
            }
            .event-label {
                font-size: 0.7rem;
                fill: #374151; /* Dark Grey */
                font-weight: 600;
            }
            /* Style for the SVG container to be responsive */
            #chart-container svg {
                display: block;
                width: 100%;
                height: 100%;
            }
            /* --- Responsive Aspect Ratio --- */
            .aspect-wrapper {
                padding-top: 66.67%; /* 3:2 aspect ratio for mobile (default) */
            }
            @media (min-width: 640px) { /* sm breakpoint */
                .aspect-wrapper {
                    padding-top: 50%; /* 2:1 aspect ratio for desktop */
                }
            }
            .time-indicator-label {
                font-size: 0.65rem;
                font-weight: 500;
            }
            /* Wave Animation for Loading */
            .wave-container {
                display: flex;
                justify-content: center;
                align-items: center;
                height: 100%;
                flex-direction: column;
                gap: 1rem;
            }
            .wave {
                display: flex;
                align-items: center;
            }
            .wave .bar {
                width: 8px;
                height: 5px;
                margin: 0 3px;
                background-color: #60a5fa;
                border-radius: 9999px;
                animation: wave-animation 1.2s ease-in-out infinite;
            }
            .wave .bar:nth-child(2) { animation-delay: 0.1s; }
            .wave .bar:nth-child(3) { animation-delay: 0.2s; }
            .wave .bar:nth-child(4) { animation-delay: 0.3s; }
            @keyframes wave-animation {
                0% { transform: scaleY(1); }
                50% { transform: scaleY(4); }
                100% { transform: scaleY(1); }
            }
        </style>

                <div class="tide-chart-container bg-transparent text-gray-800 rounded-2xl p-4 sm:p-6 w-full max-w-[600px] mx-auto">
        <!-- Header: Location, Date, and Navigation -->
        <div class="flex justify-between items-start"> <!-- Aligned to top and removed padding -->
            <button id="prev-day" class="p-2 rounded-full hover:bg-gray-100 transition-colors duration-200">
                <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                    <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 19l-7-7 7-7" />
                </svg>
            </button>
            <div class="text-center">
                 <div class="relative">
                     <div class="flex items-center justify-center space-x-2 p-1">
                         <p id="current-date" class="text-sm text-gray-600"></p>
                         <svg xmlns="http://www.w3.org/2000/svg" class="h-5 w-5 text-gray-400" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                             <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M8 7V3m8 4V3m-9 8h10M5 21h14a2 2 0 002-2V7a2 2 0 00-2-2H5a2 2 0 00-2 2v12a2 2 0 002 2z" />
                         </svg>
                     </div>
                     <input type="date" id="date-picker" class="absolute inset-0 w-full h-full opacity-0 cursor-pointer">
                 </div>
                 <p id="current-tide-status" class="text-lg text-gray-700 font-semibold h-7"></p>
                 <p id="next-tide-event" class="text-sm text-gray-500 h-5"></p>
                 <p id="club-status-alert" class="text-sm text-red-500 h-5"></p>
            </div>
            <button id="next-day" class="p-2 rounded-full hover:bg-gray-100 transition-colors duration-200">
                <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                    <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 5l7 7-7 7" />
                </svg>
            </button>
        </div>

        <!-- Aspect Ratio Wrapper -->
        <div class="relative w-full aspect-wrapper">
            <div id="chart-container" class="absolute top-0 left-0 w-full h-full"></div>
        </div>
        
        <!-- Legend -->
        <div id="legend" class="flex flex-wrap justify-center items-center gap-x-4 gap-y-1 text-xs text-gray-500 mt-2"></div>
        
        <!-- Events List -->
        <div id="events-list" class="mt-4 text-sm text-gray-600 space-y-1"></div>

        <!-- Tooltip for Hover Interaction -->
        <div id="tooltip" class="absolute pointer-events-none opacity-0 transition-opacity duration-200 bg-white bg-opacity-80 backdrop-blur-sm text-gray-800 border border-gray-200 text-sm rounded-lg px-3 py-2 shadow-lg">
            <p id="tooltip-time"></p>
            <p id="tooltip-height" class="font-bold"></p>
        </div>

    </div>

    <script>
        /*
        CHEETAH TEMPLATE DIRECTIVE:
        The #raw directive tells the Cheetah templating engine to treat the
        following block of code as raw text and not to parse it. This is
        necessary because Cheetah tries to interpret JavaScript as Python,
        which causes a syntax error.
        When deploying to your weewx server, you must remove the comment markers
        so that Cheetah can read the directives.
        
        For Cheetah, it should look like this:
        #raw
        ... your javascript code ...
        #end raw
        */
        
        /* #raw */
        document.addEventListener('DOMContentLoaded', () => {
            // --- CONFIGURATION ---
            const initialDate = new Date(); // Use the actual current date
            const DANGEROUS_LOW_TIDE = 1.5; // in feet

            // --- STATE ---
            const chartContainer = d3.select("#chart-container");
            const tooltip = d3.select("#tooltip");
            const dateDisplay = document.getElementById('current-date');
            const datePicker = document.getElementById('date-picker');
            const nextTideDisplay = document.getElementById('next-tide-event');
            const clubStatusAlert = document.getElementById('club-status-alert');
            const currentTideStatus = document.getElementById('current-tide-status');
            const eventsList = document.getElementById('events-list');
            let currentDate = new Date(initialDate);
            let countdownInterval = null;
            const tideDataCache = {};
            const NOAA_STATION_ID = "9414816"; // Richmond, CA
            // --- FIX: Updated coordinates to Berkeley, CA ---
            const LOCATION_LAT = 37.8716; // Berkeley, CA Latitude
            const LOCATION_LNG = -122.2727; // Berkeley, CA Longitude
            let isHoveringOnEvent = false;
            let svg, xScale, yScale, chartDataForUpdate; // State for live updates
            
            // --- HELPERS ---
            const parseNOAADate = d3.timeParse("%Y-%m-%d %H:%M");

            // --- FIX: Added function to determine if a date is in Pacific Daylight Time ---
            function isPDT(date) {
                const year = date.getFullYear();

                // DST starts on the second Sunday in March at 2 AM
                const dstStart = new Date(year, 2, 1); // March 1st
                const firstDayOfMonth = dstStart.getDay(); // 0=Sun, 1=Mon, ...
                const firstSunday = new Date(year, 2, 1 + (7 - firstDayOfMonth) % 7);
                const secondSundayInMarch = new Date(firstSunday.getTime() + 7 * 24 * 60 * 60 * 1000);
                secondSundayInMarch.setHours(2, 0, 0, 0); 

                // DST ends on the first Sunday in November at 2 AM
                const dstEnd = new Date(year, 10, 1); // November 1st
                const firstDayOfNov = dstEnd.getDay();
                const firstSundayInNovember = new Date(year, 10, 1 + (7 - firstDayOfNov) % 7);
                firstSundayInNovember.setHours(2, 0, 0, 0);

                return date >= secondSundayInMarch && date < firstSundayInNovember;
            }

            function calculateSunriseSunset(date, latitude, longitude) {
                const toRad = Math.PI / 180;
                const dayOfYear = d => Math.floor((d - new Date(d.getFullYear(), 0, 0)) / 86400000);
                const day = dayOfYear(date);
                const B = 2 * Math.PI * (day - 81) / 364;
                const equationOfTime = 9.87 * Math.sin(2 * B) - 7.53 * Math.cos(B) - 1.5 * Math.sin(B);
                const solarDeclination = -23.44 * Math.cos(2 * Math.PI * (day + 10) / 365);
                const zenith = 90.833;
                const latRad = latitude * toRad;
                const decRad = solarDeclination * toRad;
                const hourAngleRad = Math.acos(Math.cos(zenith * toRad) / (Math.cos(latRad) * Math.cos(decRad)) - Math.tan(latRad) * Math.tan(decRad));
                const hourAngle = hourAngleRad * (180 / Math.PI);

                // --- FIX: Use a fixed timezone for Berkeley (PST/PDT) instead of the user's local timezone ---
                // PDT is UTC-7 (-420 minutes), PST is UTC-8 (-480 minutes)
                const berkeleyTimezoneOffsetMinutes = isPDT(date) ? -7 * 60 : -8 * 60;
                
                const solarNoonMinutes = 720 - 4 * longitude - equationOfTime + berkeleyTimezoneOffsetMinutes;
                const sunriseMinutes = solarNoonMinutes - hourAngle * 4;
                const sunsetMinutes = solarNoonMinutes + hourAngle * 4;
                const sunrise = new Date(date);
                sunrise.setHours(Math.floor(sunriseMinutes / 60), Math.round(sunriseMinutes % 60), 0, 0);
                const sunset = new Date(date);
                sunset.setHours(Math.floor(sunsetMinutes / 60), Math.round(sunsetMinutes % 60), 0, 0);
                return { sunrise, sunset };
            }

            async function fetchTideData(date) {
                const dateString = date.toISOString().split('T')[0];
                if (tideDataCache[dateString]) return tideDataCache[dateString];
                const yyyymmdd = date.getFullYear() + ('0' + (date.getMonth() + 1)).slice(-2) + ('0' + date.getDate()).slice(-2);
                const url = `https://api.tidesandcurrents.noaa.gov/api/prod/datagetter?product=predictions&application=NOS.CO-OPS.CAT&begin_date=${yyyymmdd}&end_date=${yyyymmdd}&datum=MLLW&station=${NOAA_STATION_ID}&time_zone=lst_ldt&units=english&format=json`;
                try {
                    const response = await fetch(url);
                    if (!response.ok) throw new Error(`NOAA API request failed: ${response.statusText}`);
                    const data = await response.json();
                    if (data.error || !data.predictions) {
                        tideDataCache[dateString] = [];
                        return [];
                    }
                    const formattedData = data.predictions.map(p => ({ time: parseNOAADate(p.t), height: parseFloat(p.v) })).filter(p => p.time !== null);
                    tideDataCache[dateString] = formattedData;
                    return formattedData;
                } catch (error) {
                    console.error("Failed to fetch tide data:", error);
                    return null;
                }
            }
            
            function findTideExtremes(tideData) {
                if (!tideData || tideData.length < 3) return [];
                const extremes = [];
                
                // Find initial direction by skipping any initial flat sections
                let initialIndex = 1;
                let initialDirection = 0;
                while(initialIndex < tideData.length) {
                    initialDirection = Math.sign(tideData[initialIndex].height - tideData[initialIndex-1].height);
                    if (initialDirection !== 0) break;
                    initialIndex++;
                }

                // If the whole dataset is flat, there are no extremes
                if (initialDirection === 0) return [];

                let lastDirection = initialDirection;

                // Start scanning from where we first found a direction change
                for (let i = initialIndex; i < tideData.length - 1; i++) {
                    const currentDirection = Math.sign(tideData[i+1].height - tideData[i].height);
                    
                    // Ignore subsequent flat sections
                    if (currentDirection === 0) continue;

                    // Check for a change in direction
                    if (currentDirection !== lastDirection) {
                        if (lastDirection === 1 && currentDirection === -1) {
                            // Change from rising to falling -> High Tide at point i
                            extremes.push({ type: 'High', ...tideData[i] });
                        } else if (lastDirection === -1 && currentDirection === 1) {
                            // Change from falling to rising -> Low Tide at point i
                            extremes.push({ type: 'Low', ...tideData[i] });
                        }
                    }

                    lastDirection = currentDirection;
                }
                return extremes;
            }

            async function fetchClubStatus(date) {
                const yyyymmdd = date.getFullYear() + '-' + ('0' + (date.getMonth() + 1)).slice(-2) + '-' + ('0' + date.getDate()).slice(-2);
                const originalUrl = `https://www.cal-sailing.org/index.php?option=com_csctides&view=gettidedatalist&task=gettidedatalist&tmpl=component&output=json&tidedate=${yyyymmdd}`;
                const url = `https://corsproxy.io/?${encodeURIComponent(originalUrl)}`;

                try {
                    const response = await fetch(url);
                    if (!response.ok) throw new Error(`Cal Sailing API request failed: ${response.statusText}`);
                    
                    const rawText = await response.text();
                    // Clean potential HTML tags wrapping the JSON
                    const jsonString = rawText.replace(/<pre.*?>|<\/pre>/g, '');
                    const responseData = JSON.parse(jsonString);

                    // *** FIX: Access the nested 'data' object ***
                    const clubData = responseData.data;
                    if (!clubData) {
                        throw new Error("Malformed club status JSON: 'data' property is missing.");
                    }

                    const parseClubTime = (timeStr) => {
                        if (!timeStr) return null;
                        timeStr = timeStr.toLowerCase().trim();

                        if (timeStr === 'sunset') return 'sunset';
                        
                        const d = new Date(date);
                        if (timeStr === 'noon') {
                            d.setHours(12, 0, 0, 0);
                            return d;
                        }

                        const pm = timeStr.includes('pm');
                        const parts = timeStr.replace(/am|pm/i, '').trim().split(':');
                        let hours = parseInt(parts[0], 10);
                        const minutes = parts.length > 1 ? parseInt(parts[1], 10) : 0;

                        if (isNaN(hours) || isNaN(minutes)) return null;

                        if (pm && hours < 12) hours += 12;
                        else if (!pm && hours === 12) hours = 0; // Handle 12am
                        
                        d.setHours(hours, minutes, 0, 0);
                        return d;
                    };
                    
                    // Handle boolean or string from API
                    const isClosed = clubData.clubclosed === true || clubData.clubclosed === 'true';
                    if (isClosed) {
                        return { isClosed: true, alertMessage: 'Club Closed Due To Tide' };
                    }

                    const openTime = parseClubTime(clubData.opentime);
                    const dockTime = parseClubTime(clubData.docktime);
                    let midDayDockTime = null;
                    let midDayOpenTime = null;
                    
                    const alerts = new Set();
                    
                    // Check for restrictions like Early Close or Late Open
                    if (clubData.restriction) {
                        if (clubData.restriction.toUpperCase().includes('EARLY CLOSE')) {
                            alerts.add('Early Close Due To Tide');
                        }
                        if (clubData.restriction.toUpperCase().includes('LATE OPEN')) {
                            alerts.add('Late Open Due To Tide');
                        }
                    }

                    // Check for mid-day closure
                    if (clubData.middayclosetext && clubData.middayclosetext !== 'null') {
                        alerts.add('Mid-Day Closure Due To Tide');
                        const times = clubData.middayclosetext.match(/(\d{1,2}:\d{2}\s*(am|pm))/gi);
                        if (times && times.length >= 2) {
                            midDayDockTime = parseClubTime(times[0]);
                            midDayOpenTime = parseClubTime(times[1]);
                        }
                    }

                    const alertMessage = Array.from(alerts).join(' & ');

                    return {
                        isClosed: false,
                        openTime,
                        dockTime,
                        midDayDockTime,
                        midDayOpenTime,
                        alertMessage
                    };

                } catch (error) {
                    console.error("Failed to fetch or parse club status:", error);
                    return { isClosed: true, alertMessage: 'Could not load club status' };
                }
            }

            async function drawChart() {
                const loadingHTML = `
                    <div class='wave-container'>
                        <div class='text-gray-500'>Loading tide data...</div>
                        <div class='wave'>
                            <div class='bar'></div>
                            <div class='bar'></div>
                            <div class='bar'></div>
                            <div class='bar'></div>
                        </div>
                    </div>`;
                chartContainer.html(loadingHTML);
                clubStatusAlert.textContent = '';
                eventsList.innerHTML = '';

                const [currentTides, clubStatus] = await Promise.all([
                    fetchTideData(currentDate),
                    fetchClubStatus(currentDate)
                ]);
                
                chartContainer.html("");

                if (!currentTides || currentTides.length === 0) {
                    const message = !currentTides ? "Could not load tide data." : "No tide data available for this date.";
                    chartContainer.html(`<div class='text-red-500 p-4 flex items-center justify-center h-full'>${message}</div>`);
                    return;
                }
                
                const chartData = currentTides;
                chartDataForUpdate = chartData; // Assign for external access
                const startOfDay = d3.timeDay.floor(currentDate);
                const endOfDay = d3.timeDay.ceil(currentDate);

                const prevDay = new Date(currentDate); prevDay.setDate(prevDay.getDate() - 1);
                const nextDay = new Date(currentDate); nextDay.setDate(nextDay.getDate() + 1);
                const [prevTides, nextTides] = await Promise.all([ fetchTideData(prevDay), fetchTideData(nextDay) ]);
                const allTidesData = [...(prevTides || []), ...currentTides, ...(nextTides || [])];
                const allExtremes = findTideExtremes(allTidesData);
                const tideEvents = allExtremes.filter(event => event.time >= startOfDay && event.time < endOfDay);

                const containerRect = chartContainer.node().getBoundingClientRect();
                const isMobile = window.innerWidth < 640;
                const margin = { top: isMobile ? 40 : 20, right: 20, bottom: 40, left: 45 };
                const width = containerRect.width - margin.left - margin.right;
                const height = containerRect.height - margin.top - margin.bottom;

                svg = chartContainer.append("svg").attr("width", containerRect.width).attr("height", containerRect.height)
                    .append("g").attr("transform", `translate(${margin.left},${margin.top})`);
                
                xScale = d3.scaleTime().domain([startOfDay, endOfDay]).range([0, width]);
                const [minTide, maxTide] = d3.extent(chartData, d => d.height);
                yScale = d3.scaleLinear().domain([minTide - 1, maxTide + 2]).range([height, 0]);
                const curve = d3.curveBasis;
                const areaGenerator = (y0) => d3.area().x(d => xScale(d.time)).y0(y0).y1(d => yScale(d.height)).curve(curve);
                const lineGenerator = d3.line().x(d => xScale(d.time)).y(d => yScale(d.height)).curve(curve);
                
                const nightColor = "#f3f4f6", dayColor = "#FFFFFF";
                const { sunrise: sunriseTime, sunset: sunsetTime } = calculateSunriseSunset(currentDate, LOCATION_LAT, LOCATION_LNG);

                if (clubStatus.dockTime === 'sunset') {
                    clubStatus.dockTime = sunsetTime;
                }
                if (clubStatus.midDayDockTime === 'sunset') {
                    clubStatus.midDayDockTime = sunsetTime;
                }

                const overlap = 1;
                svg.append("rect").attr("x", xScale(startOfDay)).attr("width", (xScale(sunriseTime) - xScale(startOfDay)) + overlap).attr("height", height).attr("fill", nightColor);
                svg.append("rect").attr("x", xScale(sunriseTime)).attr("width", (xScale(sunsetTime) - xScale(sunriseTime)) + overlap).attr("height", height).attr("fill", dayColor);
                svg.append("rect").attr("x", xScale(sunsetTime)).attr("width", xScale(endOfDay) - xScale(sunsetTime)).attr("height", height).attr("fill", nightColor);

                const redAreaGenerator = d3.area().x(d => xScale(d.time)).y0(yScale(DANGEROUS_LOW_TIDE)).y1(d => yScale(d.height)).curve(curve);
                const redSegments = []; let currentSegment = [];
                for (let i = 0; i < chartData.length; i++) {
                    const d = chartData[i], prev = i > 0 ? chartData[i - 1] : null;
                    const isBelow = d.height < DANGEROUS_LOW_TIDE, wasBelow = prev ? prev.height < DANGEROUS_LOW_TIDE : false;
                    if (prev && isBelow !== wasBelow) {
                        const t0 = prev.time.getTime(), t1 = d.time.getTime(), h0 = prev.height, h1 = d.height;
                        const intersectionTime = new Date(t0 + (t1 - t0) * (DANGEROUS_LOW_TIDE - h0) / (h1 - h0));
                        const intersectionPoint = { time: intersectionTime, height: DANGEROUS_LOW_TIDE };
                        if (isBelow) currentSegment.push(intersectionPoint);
                        else { currentSegment.push(intersectionPoint); if (currentSegment.length > 1) redSegments.push(currentSegment); currentSegment = []; }
                    }
                    if (isBelow) currentSegment.push(d);
                }
                const lastDataPoint = chartData.length > 0 ? chartData[chartData.length - 1] : null;
                if (lastDataPoint && lastDataPoint.height < DANGEROUS_LOW_TIDE && currentSegment.length > 0) {
                    if (lastDataPoint.time < endOfDay) currentSegment.push({ time: endOfDay, height: lastDataPoint.height });
                }
                if (currentSegment.length > 1) redSegments.push(currentSegment);

                const dayEvents = [];
                clubStatusAlert.textContent = clubStatus.alertMessage || '';

                if (!clubStatus.isClosed) {
                    const openOverlayColor = "rgba(34, 197, 94, 0.15)";
                    
                    const openPeriods = [];
                    if (clubStatus.midDayDockTime && clubStatus.midDayOpenTime) {
                        if (clubStatus.openTime && clubStatus.midDayDockTime > clubStatus.openTime) {
                           openPeriods.push({ start: clubStatus.openTime, end: clubStatus.midDayDockTime, type: 'morning' });
                        }
                        if (clubStatus.midDayOpenTime && clubStatus.dockTime > clubStatus.midDayOpenTime) {
                           openPeriods.push({ start: clubStatus.midDayOpenTime, end: clubStatus.dockTime, type: 'afternoon' });
                        }
                    } 
                    else if (clubStatus.openTime && clubStatus.dockTime > clubStatus.openTime) {
                        openPeriods.push({ start: clubStatus.openTime, end: clubStatus.dockTime, type: 'full_day' });
                    }

                    openPeriods.forEach(({start, end, type}) => {
                         svg.append("rect")
                            .attr("x", xScale(start))
                            .attr("width", xScale(end) - xScale(start))
                            .attr("height", height)
                            .attr("fill", openOverlayColor);
                        
                        const openText = type === 'afternoon' ? "Club Re-Opens" : "Club Opens";
                        const dockText = type === 'morning' ? "Mid-Day Dock Time" : "Dock Time (Club Closes)";

                        if (start > startOfDay && start < endOfDay) {
                            svg.append("line").attr("x1", xScale(start)).attr("x2", xScale(start)).attr("y1", 0).attr("y2", height).attr("stroke", "#22c55e").attr("stroke-width", 1.5);
                            dayEvents.push({time: start, text: openText, type: 'open'});
                        }
                        if (end > start && end < endOfDay) {
                             svg.append("line").attr("x1", xScale(end)).attr("x2", xScale(end)).attr("y1", 0).attr("y2", height).attr("stroke", "#f97316").attr("stroke-width", 1.5);
                             dayEvents.push({time: end, text: dockText, type: 'dock'});
                        }
                    });
                }

                const risingColor = "#5da7d1", fallingColor = "#0369a1";
                const eventPoints = [chartData[0], ...tideEvents, chartData[chartData.length - 1]].sort((a, b) => a.time - b.time).filter((d, i, arr) => !d || i === 0 || d.time.getTime() !== arr[i-1].time.getTime());
                const segments = [];
                for (let i = 0; i < eventPoints.length - 1; i++) {
                    const startPoint = eventPoints[i], endPoint = eventPoints[i+1];
                    if (!startPoint || !endPoint) continue;
                    segments.push({ rising: endPoint.height > startPoint.height, data: chartData.filter(d => d.time >= startPoint.time && d.time <= endPoint.time) });
                }
                if (segments.length > 0) {
                    const lastSegment = segments[segments.length - 1], lastDataPoint = chartData[chartData.length - 1];
                    if (lastDataPoint.time < endOfDay) lastSegment.data.push({ time: endOfDay, height: lastDataPoint.height });
                }
                segments.forEach(seg => svg.append("path").datum(seg.data).attr("fill", seg.rising ? risingColor : fallingColor).attr("d", areaGenerator(height)));
                svg.append("path").datum(chartData).attr("fill", "none").attr("stroke", "#0ea5e9").attr("stroke-width", 2).attr("d", lineGenerator);
                redSegments.forEach(segment => svg.append("path").datum(segment).attr("fill", "rgba(239, 68, 68, 0.7)").attr("d", redAreaGenerator));

                svg.append("g").attr("transform", `translate(0,${height})`).call(d3.axisBottom(xScale).ticks(d3.timeHour.every(6)).tickFormat(d3.timeFormat("%H:%M"))).attr("class", "axis x-axis");
                const yAxis = d3.axisLeft(yScale), ticks = yScale.ticks(), uniqueRoundedTicks = [...new Set(ticks.map(t => Math.round(t)))];
                yAxis.tickValues(uniqueRoundedTicks).tickFormat(d => `${d} ft`);
                svg.append("g").call(yAxis).attr("class", "axis y-axis");
                
                const eventsGroup = svg.selectAll(".event").data(tideEvents).enter().append("g").attr("class", "event").attr("transform", d => `translate(${xScale(d.time)},${yScale(d.height)})`);
                eventsGroup.append("circle").attr("r", 5).attr("fill", "#f3f4f6").attr("stroke", "#1f2937").attr("stroke-width", 1.5);
                eventsGroup.each(function(d) {
                    d3.select(this).append("text").attr("class", "event-label").attr("dy", "-10px").attr("text-anchor", "middle").text(`${d.height.toFixed(2)}ft`);
                });

                updateNowIndicator(); // Draw the 'now' indicator for the first time

                eventsGroup.on("mouseover", (event, d) => { isHoveringOnEvent = true; tooltip.style("opacity", 1).style("left", `${event.pageX + 15}px`).style("top", `${event.pageY - 15}px`); tooltip.select("#tooltip-time").text(`Event: ${d3.timeFormat("%H:%M")(d.time)}`); tooltip.select("#tooltip-height").text(`${d.height.toFixed(2)} ft`); }).on("mouseout", () => { isHoveringOnEvent = false; tooltip.style("opacity", 0); });

                const hoverGroup = svg.append("g").attr("class", "hover-group").style("display", "none");
                hoverGroup.append("line").attr("stroke", "#9ca3af").attr("stroke-width", 1).attr("y1", 0).attr("y2", height);
                const hoverCircle = hoverGroup.append("circle").attr("r", 5).attr("fill", "#ef4444").attr("stroke", "#ffffff").attr("stroke-width", 1.5);
                function handleInteractionMove(event) {
                    event.preventDefault();
                    if (isHoveringOnEvent) { hoverGroup.style("display", "none"); return; }
                    hoverGroup.style("display", null);
                    const pointer = event.touches ? event.touches[0] : event;
                    const [mx] = d3.pointer(pointer, this);
                    const time = xScale.invert(mx);
                    const bisect = d3.bisector(d => d.time).left;
                    const index = bisect(chartData, time, 1);
                    const a = chartData[index - 1], b = chartData[index];
                    let d = !a ? b : !b ? a : (time - a.time > b.time - time) ? b : a;
                    if (!d) return;
                    hoverGroup.attr("transform", `translate(${xScale(d.time)}, 0)`);
                    hoverCircle.attr("cy", yScale(d.height));
                    const tooltipNode = tooltip.node(), tooltipRect = tooltipNode.getBoundingClientRect();
                    let leftPos = pointer.pageX + 15, topPos = pointer.pageY - 15;
                    if (event.touches) { leftPos = pointer.pageX - (tooltipRect.width / 2); topPos = pointer.pageY - tooltipRect.height - 20; }
                    tooltip.style("left", `${leftPos}px`).style("top", `${topPos}px`);
                    tooltip.select("#tooltip-time").text(d3.timeFormat("%H:%M")(d.time));
                    tooltip.select("#tooltip-height").text(`${d.height.toFixed(2)} ft`);
                }
                svg.append("rect").attr("width", width).attr("height", height).attr("fill", "none").attr("pointer-events", "all")
                    .on("mouseover", (e) => { if (!isHoveringOnEvent) { hoverGroup.style("display", null); tooltip.style("opacity", 1); }})
                    .on("mouseout", (e) => { hoverGroup.style("display", "none"); if (!isHoveringOnEvent) tooltip.style("opacity", 0); })
                    .on("mousemove", handleInteractionMove)
                    .on("touchstart", (e) => { e.preventDefault(); if (!isHoveringOnEvent) { hoverGroup.style("display", null); tooltip.style("opacity", 1); }}, { passive: false })
                    .on("touchmove", handleInteractionMove, { passive: false })
                    .on("touchend", (e) => { hoverGroup.style("display", "none"); if (!isHoveringOnEvent) tooltip.style("opacity", 0);})
                    .on("touchcancel", (e) => { hoverGroup.style("display", "none"); if (!isHoveringOnEvent) tooltip.style("opacity", 0);});
                
                tideEvents.forEach(event => {
                    if (event.time >= sunriseTime && event.time <= sunsetTime) {
                        dayEvents.push({ time: event.time, text: `${event.type} Tide: ${event.height.toFixed(2)} ft`, type: event.type.toLowerCase() });
                    }
                });
                
                dayEvents.push({ time: sunsetTime, text: "Sunset", type: 'sunset' });
                dayEvents.sort((a, b) => a.time - b.time);
                
                const uniqueEvents = [...new Set(dayEvents.map(e => JSON.stringify(e)))].map(s => JSON.parse(s));
                uniqueEvents.sort((a,b) => new Date(a.time) - new Date(b.time));

                if (uniqueEvents.length > 0) {
                     const eventIcons = {
                          open: `<svg class="w-4 h-4 text-green-500" fill="none" viewBox="0 0 24 24" stroke="currentColor"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M14.752 11.168l-3.197-2.132A1 1 0 0010 9.87v4.263a1 1 0 001.555.832l3.197-2.132a1 1 0 000-1.664z"></path><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M21 12a9 9 0 11-18 0 9 9 0 0118 0z"></path></svg>`,
                          dock: `<svg class="w-4 h-4 text-orange-500" fill="none" viewBox="0 0 24 24" stroke="currentColor"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M10 9v6m4-6v6m7-3a9 9 0 11-18 0 9 9 0 0118 0z"></path></svg>`,
                          high: `<svg class="w-4 h-4 text-cyan-500" fill="none" viewBox="0 0 24 24" stroke="currentColor"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M5 10l7-7m0 0l7 7m-7-7v18"></path></svg>`,
                          low: `<svg class="w-4 h-4 text-blue-500" fill="none" viewBox="0 0 24 24" stroke="currentColor"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M19 14l-7 7m0 0l-7-7m7 7V3"></path></svg>`,
                          sunset: `<svg class="w-4 h-4 text-orange-500" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" viewBox="0 0 24 24"><path d="M17 18a5 5 0 00-10 0M12 2v7m-5.66 2.34l1.42 1.42M1 18h2m18 0h2M18.36 11.64l1.42-1.42M23 22H1m15-17l-4 4-4-4"/></svg>`
                     };
                    
                     let tableHTML = '<table class="w-full text-left">';
                     tableHTML += '<tbody>';
                     tableHTML += uniqueEvents.map(e => {
                         const icon = eventIcons[e.type] || '';
                         return `<tr class="border-t border-gray-200"><td class="py-1 w-6">${icon}</td><td class="pr-4 py-1">${e.text}</td><td class="py-1 text-right"><strong class="font-medium">${d3.timeFormat("%H:%M")(new Date(e.time))}</strong></td></tr>`;
                     }).join('');
                     tableHTML += '</tbody></table>';
                     eventsList.innerHTML = tableHTML;
                } else {
                     eventsList.innerHTML = `<p>No scheduled events during daylight hours.</p>`;
                }

                updateUI();
            }

            function updateNowIndicator() {
                if (!svg || !xScale || !yScale || !chartDataForUpdate) return;

                // Remove previous indicator to prevent duplicates
                svg.select(".now-indicator").remove();

                const now = new Date();
                const startOfDay = d3.timeDay.floor(currentDate);
                const endOfDay = d3.timeDay.ceil(currentDate);

                // Only draw if we are on the current day
                if (currentDate.toDateString() === now.toDateString() && now >= startOfDay && now <= endOfDay) {
                    const bisect = d3.bisector(d => d.time).left;
                    const index = bisect(chartDataForUpdate, now, 1);
                    const a = chartDataForUpdate[index - 1];
                    const b = chartDataForUpdate[index];
                    // Find the closest data point in time to 'now'
                    const currentDataPoint = !a ? b : !b ? a : (now - a.time > b.time - now) ? b : a;
                    
                    if (currentDataPoint) {
                         svg.append("circle")
                            .attr("class", "now-indicator") // Add class for easy selection/removal
                            .attr("cx", xScale(now))
                            .attr("cy", yScale(currentDataPoint.height))
                            .attr("r", 5)
                            .attr("fill", "#facc15")
                            .style("pointer-events", "none");
                    }
                }
            }

            function updateUI() {
                const isMobile = window.innerWidth < 640;
                const dateFormatOptions = { weekday: 'long', month: 'long', day: 'numeric' };
                if (!isMobile) dateFormatOptions.year = 'numeric';
                dateDisplay.textContent = currentDate.toLocaleString(undefined, dateFormatOptions);

                const yyyy = currentDate.getFullYear(), mm = String(currentDate.getMonth() + 1).padStart(2, '0'), dd = String(currentDate.getDate()).padStart(2, '0');
                datePicker.value = `${yyyy}-${mm}-${dd}`;
                if(countdownInterval) clearInterval(countdownInterval);
                const isCurrentDay = currentDate.toDateString() === new Date().toDateString();
                nextTideDisplay.style.display = isCurrentDay ? 'block' : 'none';
                currentTideStatus.style.display = isCurrentDay ? 'block' : 'none';
                if (isCurrentDay) {
                    updateNextTideEvent();
                    countdownInterval = setInterval(updateNextTideEvent, 60000); 
                }
                document.getElementById('legend').innerHTML = `<div class="flex items-center space-x-1"><div class="w-3 h-3 rounded-sm" style="background-color: rgba(34, 197, 94, 0.15);"></div><span>Club Open</span></div><div class="flex items-center space-x-1"><div class="w-0.5 h-3 bg-green-500"></div><span>Open Time</span></div> <div class="flex items-center space-x-1"><div class="w-0.5 h-3 bg-orange-500"></div><span>Dock Time</span></div> <div class="flex items-center space-x-1"><div class="w-3 h-3 rounded-full" style="background-color: #facc15;"></div><span>Now</span></div><div class="flex items-center space-x-1"><div class="w-3 h-3" style="background-color: rgba(239, 68, 68, 0.7);"></div><span>Club Closed < 1.5' Tide</span></div>`;
            }

            async function updateNextTideEvent() {
                try {
                    const now = new Date(); // Use actual current time
                    const [prevTides, currentTides, nextTides, nextNextTides] = await Promise.all([ 
                        fetchTideData(new Date(now.getFullYear(), now.getMonth(), now.getDate() - 1)), 
                        fetchTideData(now), 
                        fetchTideData(new Date(now.getFullYear(), now.getMonth(), now.getDate() + 1)),
                        fetchTideData(new Date(now.getFullYear(), now.getMonth(), now.getDate() + 2)) 
                    ]);

                    if (!prevTides || !currentTides || !nextTides || !nextNextTides) { 
                        nextTideDisplay.innerHTML = ""; 
                        currentTideStatus.innerHTML = "";
                        return; 
                    }
                    
                    // FIX: Using a wider range of data for lookups to make it more robust.
                    const combinedTideDataForLookup = [...prevTides, ...currentTides, ...nextTides];
                    const bisect = d3.bisector(d => d.time).left;
                    
                    const index = bisect(combinedTideDataForLookup, now, 1);
                    const a = combinedTideDataForLookup[index - 1];
                    const b = combinedTideDataForLookup[index];
                    const currentTidePoint = !a ? b : !b ? a : (now - a.time > b.time - now) ? b : a;

                    if (currentTidePoint) {
                        const currentHeight = currentTidePoint.height;
                        const timeIn5Mins = new Date(now.getTime() + 5 * 60000);
                        const indexIn5Mins = bisect(combinedTideDataForLookup, timeIn5Mins, 1);
                        const a5 = combinedTideDataForLookup[indexIn5Mins - 1];
                        const b5 = combinedTideDataForLookup[indexIn5Mins];
                        const futureTidePoint = !a5 ? b5 : !b5 ? a5 : (timeIn5Mins - a5.time > b.time - timeIn5Mins) ? b5 : a5;
                        
                        let directionArrow = '';
                        if (futureTidePoint) {
                            const futureHeight = futureTidePoint.height;
                            if (futureHeight > currentHeight) directionArrow = `<svg class="w-5 h-5 text-cyan-500 inline-block align-middle" fill="none" viewBox="0 0 24 24" stroke="currentColor" stroke-width="2"><path stroke-linecap="round" stroke-linejoin="round" d="M5 10l7-7m0 0l7 7m-7-7v18"></path></svg>`;
                            else if (futureHeight < currentHeight) directionArrow = `<svg class="w-5 h-5 text-blue-500 inline-block align-middle" fill="none" viewBox="0 0 24 24" stroke="currentColor" stroke-width="2"><path stroke-linecap="round" stroke-linejoin="round" d="M19 14l-7 7m0 0l-7-7m7 7V3"></path></svg>`;
                        }
                        currentTideStatus.innerHTML = `<span class="inline-flex items-center">${directionArrow}<span class="ml-1">${currentHeight.toFixed(2)} ft</span></span>`;
                    } else {
                        currentTideStatus.innerHTML = "";
                    }

                    // FIX: Combine raw data from multiple days before finding extremes
                    const allRawTides = [...prevTides, ...currentTides, ...nextTides, ...nextNextTides];
                    const allTideExtremes = findTideExtremes(allRawTides);
                    const nextEvent = allTideExtremes.find(event => event.time > now);
                    
                    if (nextEvent) {
                        const diff = nextEvent.time - now, hours = Math.floor(diff / 3600000), minutes = Math.floor((diff % 3600000) / 60000);
                        nextTideDisplay.innerHTML = `<span>${nextEvent.type} tide in ${hours}h ${minutes}m at ${nextEvent.height.toFixed(2)} ft</span>`;
                    } else {
                        nextTideDisplay.innerHTML = "";
                    }
                    updateNowIndicator(); // Update the cursor position every minute
                } catch (error) {
                    console.error("Error during periodic tide update:", error);
                    // Clear the fields on error to avoid showing stale data
                    currentTideStatus.innerHTML = "";
                    nextTideDisplay.innerHTML = "<span>Could not update status</span>";
                }
            }

            function clearHeaderInfo() {
                dateDisplay.textContent = '';
                currentTideStatus.innerHTML = '';
                nextTideDisplay.innerHTML = '';
                clubStatusAlert.textContent = '';
            }

            document.getElementById('prev-day').addEventListener('click', () => {
                currentDate.setDate(currentDate.getDate() - 1);
                clearHeaderInfo();
                drawChart();
            });
            document.getElementById('next-day').addEventListener('click', () => {
                currentDate.setDate(currentDate.getDate() + 1);
                clearHeaderInfo();
                drawChart();
            });
            datePicker.addEventListener('change', (event) => {
                const dateString = event.target.value;
                if (!dateString) return;
                const [year, month, day] = dateString.split('-').map(Number);
                const selectedDate = new Date(currentDate.getTime());
                selectedDate.setFullYear(year, month - 1, day);
                if (!isNaN(selectedDate.getTime()) && selectedDate.toDateString() !== currentDate.toDateString()) {
                    currentDate = selectedDate;
                    clearHeaderInfo();
                    drawChart();
                }
            });
            window.addEventListener('resize', drawChart);

            drawChart();
        });
        /* #end raw */
    </script>


                    </div><br/><br/>



       <h3><strong>Webcams</strong></h3><br/>             


            #if len($radar_externals_list) > 1
            <bx-tabs trigger-content="Select an item" value="$radar_externals_list[0]" type="container">
              #if 'radar_html' in $Extras
              <bx-tab id="tab-radar" target="panel-radar" value="radar">
                $gettext("Recent radar")
              </bx-tab>
              #end if

              #if len($externals_list) > 0
                #for $external in $externals_list
                  <bx-tab id="tab-$external" target="panel-$external" value="$external">
                    $Extras['externals'][$external].title
                  </bx-tab>
                #end for
              #end if
            </bx-tabs>
            #end if
            <div class="bx-ce-demo-devenv--tab-panels">
              #if 'radar_html' in $Extras
              <div id="panel-radar" role="tabpanel" aria-labelledby="tab-radar">
                <div class="bx--tile tile-radar-forecast">
                  $Extras.radar_html
                </div>
              </div>
              #end if

              #if len($externals_list) > 0
                #for $external in $externals_list
                  <div id="panel-$external" role="tabpanel" aria-labelledby="tab-$external">
                    <div class="bx--tile tile-radar-forecast">
                      $Extras['externals'][$external].source
                      <p class="bx--type-caption-01" style="margin-top: 0.25rem;">$Extras['externals'][$external].title_long</p>
                    </div>
                  </div>
                #end for
              #end if
            </div>


        <h3><strong>Cal Sailing Club on various Weather Services</strong></h3><br/><br/>
        <div class="bx--tile tile-radar-forecast">

        <a target="_blank" href="https://tempestwx.com/station/127241/grid">Tempest (Real-time from our Weather Station)</a><br><br>
        <a target="_blank" href="https://www.sailflow.com/spot/624475">Sailflow </a><br><br>
       <a target="_blank" href="https://wx.iwindsurf.com/spot/624475">iWindsurf</a><br><br>
       <a target="_blank" href="https://www.windalert.com/spot/624475">WindAlert</a><br><br>
        <a target="_blank" href="https://www.pwsweather.com/station/pws/calsailingclub">PWS Weather</a><br><br>
       <a target="_blank" href="https://www.windy.com/station/pws-f07682d7">Windy</a><br><br>
        <a target="_blank" href="https://www.wunderground.com/dashboard/pws/KCABERKE302">Weather Underground</a>


        </div>

            #end if
          </div>
          #end if
        </div>
      </div>
    </div>





    <br/><br/>

    #if 'table_tile_observations' in $DisplayOptions
    <div class="bx--row">
      <!-- prettier-ignore -->
      #set global $table_obs = None
      #include "includes/data-table-tile.inc"
    </div>
    #end if
