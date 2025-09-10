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
font-size:1.75rem!important;
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


           <script src="https://d3js.org/d3.v7.min.js"></script>
        
  <style>
        /* --- SCOPED CSS RESET --- */
        #tide-chart-widget-container * {
            box-sizing: border-box;
            font-family: 'Inter', sans-serif;
            -webkit-font-smoothing: antialiased;
            -moz-osx-font-smoothing: grayscale;
        }
        #tide-chart-widget-container p,
        #tide-chart-widget-container h1,
        #tide-chart-widget-container h2,
        #tide-chart-widget-container h3 {
            margin: 0;
            padding: 0;
            font-weight: normal;
        }
        #tide-chart-widget-container strong {
            font-weight: 500; /* or 600 if you prefer */
        }
        #tide-chart-widget-container button {
            background-color: transparent;
            border: none;
            padding: 0;
            margin: 0;
            cursor: pointer;
        }
        #tide-chart-widget-container svg {
             display: inline-block; /* Prevents unwanted bottom margin */
        }
        #tide-chart-widget-container table {
            border-collapse: collapse;
            border-spacing: 0;
        }


        /* --- THEME VARIABLES & BASE STYLES --- */
        #tide-chart-widget-container {
            /* Light theme variables */
            --color-page-bg: #ffffff;
            --color-text-primary: #374151;
            --color-text-secondary: #6b7280;
            --color-text-tertiary: #6b7280;
            --color-text-danger: #ef4444;
            --color-button-hover-bg: #f3f4f6;
            --color-axis-stroke: #d1d5db;
            --color-chart-day: #FFFFFF;
            --color-chart-night: #f3f4f6;
            --color-chart-open-overlay: rgba(34, 197, 94, 0.15);
            --color-chart-danger-area: rgba(239, 68, 68, 0.7);
            --color-chart-rising: #5da7d1;
            --color-chart-falling: #0369a1;
            --color-chart-tide-line: #0ea5e9;
            --color-open-line: #22c55e;
            --color-dock-line: #f97316;
            --color-event-circle-fill: #f3f4f6;
            --color-event-circle-stroke: #1f2937;
            --color-hover-line: #9ca3af;
            --color-hover-circle-fill: #ef4444;
            --color-hover-circle-stroke: #ffffff;
            --color-now-indicator: #facc15;
            --color-tooltip-bg: rgba(255, 255, 255, 0.8);
            --color-tooltip-text: #1f2937;
            --color-tooltip-border: #e5e7eb;
            --color-table-border: #e5e7eb;
            --icon-color-open: #22c55e;
            --icon-color-dock: #f97316;
            --icon-color-high: #22d3ee;
            --icon-color-low: #3b82f6;
            --icon-color-sunset: #f97316;

            /* Base widget styles */
            background-color: var(--color-page-bg);
            color: var(--color-text-primary);
            transition: background-color 0.3s, color 0.3s;
            touch-action: manipulation;
            border-radius: 1rem; /* rounded-2xl */
            width: 100%;
            max-width: 600px;
            margin-left: auto;
            margin-right: auto;
        }

        /* Dark theme variables */
        .dark #tide-chart-widget-container {
            --color-page-bg: transparent;
            --color-text-primary: #d1d5db;
            --color-text-secondary: #9ca3af;
            --color-text-tertiary: #9ca3af;
            --color-text-danger: #f87171;
            --color-button-hover-bg: #374151;
            --color-axis-stroke: #4b5563;
            --color-chart-day: #1f2937;
            --color-chart-night: #111827;
            --color-chart-open-overlay: rgba(22, 163, 74, 0.25);
            --color-chart-danger-area: rgba(220, 38, 38, 0.6);
            --color-chart-rising: #2563eb;
            --color-chart-falling: #3b82f6;
            --color-chart-tide-line: #60a5fa;
            --color-open-line: #4ade80;
            --color-dock-line: #fb923c;
            --color-event-circle-fill: #374151;
            --color-event-circle-stroke: #d1d5db;
            --color-hover-line: #6b7280;
            --color-hover-circle-fill: #f87171;
            --color-hover-circle-stroke: #111827;
            --color-now-indicator: #fde047;
            --color-tooltip-bg: rgba(17, 24, 39, 0.8);
            --color-tooltip-text: #f9fafb;
            --color-tooltip-border: #374151;
            --color-table-border: #374151;
            --icon-color-open: #4ade80;
            --icon-color-dock: #fb923c;
            --icon-color-high: #67e8f9;
            --icon-color-low: #60a5fa;
            --icon-color-sunset: #fb923c;
        }

        /* --- CUSTOM CSS STYLES --- */
        #tide-chart-widget-container .widget-header {
            display: flex;
            justify-content: space-between;
            align-items: center; /* Vertically center header content */
            padding: 0 1rem;
        }
        @media (min-width: 640px) {
            #tide-chart-widget-container .widget-header { padding: 0 1.5rem; }
        }

        #tide-chart-widget-container .nav-button {
            padding: 0.5rem;
            border-radius: 9999px;
            transition: background-color 0.2s;
        }
        #tide-chart-widget-container .nav-button:hover {
            background-color: var(--color-button-hover-bg);
        }
        #tide-chart-widget-container .nav-button svg {
            width: 1.5rem;
            height: 1.5rem;
            color: var(--color-text-secondary);
        }

        #tide-chart-widget-container .info-center { text-align: center; }
        #tide-chart-widget-container .date-picker-wrapper { position: relative; }
        #tide-chart-widget-container .date-display {
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 0.5rem;
            padding: 0.25rem;
        }
        #tide-chart-widget-container #current-date { font-size: 0.875rem; color: var(--color-text-secondary); }
        #tide-chart-widget-container .date-display svg { width: 1.25rem; height: 1.25rem; color: var(--color-text-tertiary); }
        
        #tide-chart-widget-container #date-picker {
            position: absolute;
            top: 0; right: 0; bottom: 0; left: 0;
            width: 100%; height: 100%;
            opacity: 0;
            cursor: pointer;
        }

        #tide-chart-widget-container #current-tide-status { font-size: 1.125rem; font-weight: 600; min-height: 1.75rem; color: var(--color-text-primary); }
        #tide-chart-widget-container #next-tide-event { font-size: 0.875rem; min-height: 1.25rem; color: var(--color-text-secondary); }
        #tide-chart-widget-container #club-status-alert { font-size: 0.875rem; min-height: 1.25rem; color: var(--color-text-danger); }
        /* Collapse status lines when they are empty */
        #tide-chart-widget-container #current-tide-status:empty,
        #tide-chart-widget-container #next-tide-event:empty,
        #tide-chart-widget-container #club-status-alert:empty {
            min-height: 0;
        }

        #tide-chart-widget-container .chart-aspect-wrapper { position: relative; width: 100%; }
        #tide-chart-widget-container #chart-container { position: absolute; top: 0; left: 0; width: 100%; height: 100%; }

        #tide-chart-widget-container #legend {
            display: flex;
            flex-wrap: wrap;
            justify-content: center;
            align-items: center;
            gap: 0.25rem 1rem;
            font-size: 0.75rem;
            margin-top: 0.5rem;
            padding-left: 1rem; padding-right: 1rem;
            color: var(--color-text-secondary);
        }
         #tide-chart-widget-container #legend > div { display: flex; align-items: center; gap: 0.25rem; }

        #tide-chart-widget-container #events-list {
            margin-top: 1rem;
            font-size: 0.875rem;
            padding-left: 1rem; padding-right: 1rem;
            padding-bottom: 1rem;
            color: var(--color-text-secondary);
        }
        @media (min-width: 640px) {
            #tide-chart-widget-container #legend { padding-left: 1.5rem; padding-right: 1.5rem; }
            #tide-chart-widget-container #events-list { padding-left: 1.5rem; padding-right: 1.5rem; padding-bottom: 1.5rem; }
        }
        #tide-chart-widget-container #events-list table { 
            width: 100%; 
            text-align: left;
        }
        #tide-chart-widget-container #events-list tr { border-top: 1px solid var(--color-table-border); }
        #tide-chart-widget-container #events-list td { 
            padding-top: 0.25rem; 
            padding-bottom: 0.25rem;
            vertical-align: middle; /* Align icons and text */
        }
        #tide-chart-widget-container #events-list td:first-child { width: 1.5rem; }
        #tide-chart-widget-container #events-list td:nth-child(2) { padding-right: 1rem; }
        #tide-chart-widget-container #events-list td:last-child { text-align: right; }
        #tide-chart-widget-container #events-list strong { font-weight: 500; color: var(--color-text-primary); }
        
        #tide-chart-widget-container .icon-sm { width: 1rem; height: 1rem; }
        #tide-chart-widget-container .icon-open { color: var(--icon-color-open); }
        #tide-chart-widget-container .icon-dock { color: var(--icon-color-dock); }
        #tide-chart-widget-container .icon-high { color: var(--icon-color-high); }
        #tide-chart-widget-container .icon-low { color: var(--icon-color-low); }
        #tide-chart-widget-container .icon-sunset { color: var(--icon-color-sunset); }


        #tide-chart-widget-container #tooltip {
            position: absolute;
            pointer-events: none;
            opacity: 0;
            transition: opacity 0.2s;
            backdrop-filter: blur(4px);
            -webkit-backdrop-filter: blur(4px);
            font-size: 0.875rem;
            border-radius: 0.5rem;
            padding: 0.5rem 0.75rem;
            box-shadow: 0 4px 6px -1px rgb(0 0 0 / 0.1), 0 2px 4px -2px rgb(0 0 0 / 0.1);
            background-color: var(--color-tooltip-bg);
            color: var(--color-tooltip-text);
            border: 1px solid var(--color-tooltip-border);
        }
        #tide-chart-widget-container #tooltip-height { font-weight: 700; }

        #tide-chart-widget-container .axis path,
        #tide-chart-widget-container .axis line { stroke: var(--color-axis-stroke); }
        #tide-chart-widget-container .axis text { fill: var(--color-text-tertiary); font-size: 0.75rem; }
        #tide-chart-widget-container .event-label { font-size: 0.7rem; fill: var(--color-text-primary); font-weight: 600; }
        #tide-chart-widget-container #chart-container svg { display: block; width: 100%; height: 100%; }
        #tide-chart-widget-container .aspect-wrapper { padding-top: 66.67%; position: relative; width: 100%; }
        @media (min-width: 640px) { #tide-chart-widget-container .aspect-wrapper { padding-top: 50%; } }
        #tide-chart-widget-container .wave-container { display: flex; justify-content: center; align-items: center; height: 100%; flex-direction: column; gap: 1rem; }
        #tide-chart-widget-container .wave .bar { width: 8px; height: 5px; margin: 0 3px; background-color: #60a5fa; border-radius: 9999px; animation: wave-animation 1.2s ease-in-out infinite; }
        #tide-chart-widget-container .wave .bar:nth-child(2) { animation-delay: 0.1s; }
        #tide-chart-widget-container .wave .bar:nth-child(3) { animation-delay: 0.2s; }
        #tide-chart-widget-container .wave .bar:nth-child(4) { animation-delay: 0.3s; }
        
        @keyframes wave-animation {
            0% { transform: scaleY(1); } 50% { transform: scaleY(4); } 100% { transform: scaleY(1); }
        }
    </style>

       <div id="tide-chart-widget-container">
        <div class="widget-header">
            <button id="prev-day" class="nav-button">
                <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                    <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 19l-7-7 7-7" />
                </svg>
            </button>
            <div class="info-center">
                 <div class="date-picker-wrapper">
                     <div class="date-display">
                         <p id="current-date"></p>
                         <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                             <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M8 7V3m8 4V3m-9 8h10M5 21h14a2 2 0 002-2V7a2 2 0 00-2-2H5a2 2 0 00-2 2v12a2 2 0 002 2z" />
                         </svg>
                     </div>
                     <input type="date" id="date-picker">
                 </div>
                 <p id="current-tide-status"></p>
                 <p id="next-tide-event"></p>
                 <p id="club-status-alert"></p>
            </div>
            <button id="next-day" class="nav-button">
                <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                    <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 5l7 7-7 7" />
                </svg>
            </button>
        </div>

        <div class="aspect-wrapper">
            <div id="chart-container" style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></div>
        </div>
        
        <div id="legend"></div>
        
        <div id="events-list"></div>

    </div>

    <!-- Tooltip is now outside the main container to avoid stacking context issues -->
    <div id="tide-chart-tooltip" style="display: none;">
        <p id="tooltip-time"></p>
        <p id="tooltip-height"></p>
    </div>

    <script>
        /* #raw */
        document.addEventListener('DOMContentLoaded', () => {
            const initialDate = new Date();
            const DANGEROUS_LOW_TIDE = 1.5;
            const chartContainer = d3.select("#chart-container");
            const dateDisplay = document.getElementById('current-date');
            const datePicker = document.getElementById('date-picker');
            const nextTideDisplay = document.getElementById('next-tide-event');
            const clubStatusAlert = document.getElementById('club-status-alert');
            const currentTideStatus = document.getElementById('current-tide-status');
            const eventsList = document.getElementById('events-list');
            
            // --- Tooltip Handling ---
            let tooltipNode = document.getElementById('tide-chart-tooltip');
            if (tooltipNode && tooltipNode.parentElement !== document.body) {
                document.body.appendChild(tooltipNode);
            }
            tooltipNode.style.display = 'block';
            const tooltip = d3.select("#tide-chart-tooltip");
            
            let currentDate = new Date(initialDate);
            let countdownInterval = null;
            const tideDataCache = {};
            const NOAA_STATION_ID = "9414816";
            const LOCATION_LAT = 37.8716;
            const LOCATION_LNG = -122.2727;
            let isHoveringOnEvent = false;
            let svg, xScale, yScale, chartDataForUpdate;

            // --- THEME REACTIVITY ---
            const redrawChartOnThemeChange = () => {
                if (chartContainer.node() && chartContainer.select("svg").node()) {
                    drawChart();
                }
            };
            const observer = new MutationObserver((mutationsList) => {
                for (const mutation of mutationsList) {
                    if (mutation.type === 'attributes' && mutation.attributeName === 'class') {
                        redrawChartOnThemeChange();
                    }
                }
            });
            observer.observe(document.documentElement, { attributes: true });
            window.matchMedia('(prefers-color-scheme: dark)').addEventListener('change', redrawChartOnThemeChange);

            // --- HELPERS ---
            const parseNOAADate = d3.timeParse("%Y-%m-%d %H:%M");
            function isPDT(date) {
                const year = date.getFullYear();
                const dstStart = new Date(year, 2, 1);
                const firstDayOfMonth = dstStart.getDay();
                const firstSunday = new Date(year, 2, 1 + (7 - firstDayOfMonth) % 7);
                const secondSundayInMarch = new Date(firstSunday.getTime() + 7 * 24 * 60 * 60 * 1000);
                secondSundayInMarch.setHours(2, 0, 0, 0); 
                const dstEnd = new Date(year, 10, 1);
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
                
                const maxRetries = 10;
                const retryDelay = 1000; 

                for (let attempt = 1; attempt <= maxRetries; attempt++) {
                    try {
                        const response = await fetch(url);
                        if (!response.ok) throw new Error(`NOAA API request failed: ${response.statusText} (Attempt ${attempt})`);
                        const data = await response.json();
                        if (data.error || !data.predictions) {
                            console.warn(`NOAA returned no predictions for ${dateString} (Attempt ${attempt})`);
                            tideDataCache[dateString] = [];
                            return [];
                        }
                        const formattedData = data.predictions.map(p => ({ time: parseNOAADate(p.t), height: parseFloat(p.v) })).filter(p => p.time !== null);
                        tideDataCache[dateString] = formattedData;
                        return formattedData;
                    } catch (error) {
                        console.error(`Failed to fetch tide data on attempt ${attempt}:`, error);
                        if (attempt < maxRetries) {
                            await new Promise(resolve => setTimeout(resolve, retryDelay));
                        } else {
                            console.error(`All ${maxRetries} attempts to fetch tide data failed.`);
                            return null;
                        }
                    }
                }
                return null; 
            }
            
            function findTideExtremes(tideData) {
                if (!tideData || tideData.length < 3) return [];
                const extremes = [];
                let initialIndex = 1;
                let initialDirection = 0;
                while(initialIndex < tideData.length) {
                    initialDirection = Math.sign(tideData[initialIndex].height - tideData[initialIndex-1].height);
                    if (initialDirection !== 0) break;
                    initialIndex++;
                }
                if (initialDirection === 0) return [];
                let lastDirection = initialDirection;
                for (let i = initialIndex; i < tideData.length - 1; i++) {
                    const currentDirection = Math.sign(tideData[i+1].height - tideData[i].height);
                    if (currentDirection === 0) continue;
                    if (currentDirection !== lastDirection) {
                        if (lastDirection === 1 && currentDirection === -1) {
                            extremes.push({ type: 'High', ...tideData[i] });
                        } else if (lastDirection === -1 && currentDirection === 1) {
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
                const maxRetries = 10;
                const retryDelay = 1000;

                for (let attempt = 1; attempt <= maxRetries; attempt++) {
                    try {
                        const response = await fetch(url);
                        if (!response.ok) throw new Error(`Cal Sailing API request failed: ${response.statusText} (Attempt ${attempt})`);
                        
                        const rawText = await response.text();
                        const jsonString = rawText.replace(/<pre.*?>|<\/pre>/g, '');
                        const responseData = JSON.parse(jsonString);
                        const clubData = responseData.data;

                        if (!clubData) throw new Error("Malformed club status JSON: 'data' property is missing.");

                        const parseClubTime = (timeStr) => {
                            if (!timeStr) return null;
                            timeStr = timeStr.toLowerCase().trim();
                            if (timeStr === 'sunset') return 'sunset';
                            const d = new Date(date);
                            if (timeStr === 'noon') { d.setHours(12, 0, 0, 0); return d; }
                            const pm = timeStr.includes('pm');
                            const parts = timeStr.replace(/am|pm/i, '').trim().split(':');
                            let hours = parseInt(parts[0], 10);
                            const minutes = parts.length > 1 ? parseInt(parts[1], 10) : 0;
                            if (isNaN(hours) || isNaN(minutes)) return null;
                            if (pm && hours < 12) hours += 12;
                            else if (!pm && hours === 12) hours = 0;
                            d.setHours(hours, minutes, 0, 0);
                            return d;
                        };
                        
                        const isClosed = clubData.clubclosed === true || clubData.clubclosed === 'true';
                        if (isClosed) return { isClosed: true, alertMessage: 'Club Closed Due To Tide' };

                        const openTime = parseClubTime(clubData.opentime);
                        const dockTime = parseClubTime(clubData.docktime);
                        let midDayDockTime = null;
                        let midDayOpenTime = null;
                        const alerts = new Set();
                        
                        if (clubData.restriction) {
                            if (clubData.restriction.toUpperCase().includes('EARLY CLOSE')) alerts.add('Early Close Due To Tide');
                            if (clubData.restriction.toUpperCase().includes('LATE OPEN')) alerts.add('Late Open Due To Tide');
                        }

                        if (clubData.middayclosetext && clubData.middayclosetext !== 'null') {
                            alerts.add('Mid-Day Closure Due To Tide');
                            const times = clubData.middayclosetext.match(/(\d{1,2}:\d{2}\s*(am|pm))/gi);
                            if (times && times.length >= 2) {
                                midDayDockTime = parseClubTime(times[0]);
                                midDayOpenTime = parseClubTime(times[1]);
                            }
                        }
                        const alertMessage = Array.from(alerts).join(' & ');
                        return { isClosed: false, openTime, dockTime, midDayDockTime, midDayOpenTime, alertMessage };

                    } catch (error) {
                        console.error(`Failed to fetch or parse club status on attempt ${attempt}:`, error);
                        if (attempt < maxRetries) {
                            await new Promise(resolve => setTimeout(resolve, retryDelay));
                        } else {
                            console.error(`All ${maxRetries} attempts to fetch club status failed.`);
                            return { isClosed: true, alertMessage: 'Could not load club status' };
                        }
                    }
                }
                return { isClosed: true, alertMessage: 'Could not load club status' };
            }

            async function drawChart() {
                const loadingHTML = `<div class='wave-container'><div style='color: var(--color-text-secondary)'>Loading tide data...</div><div class='wave'><div class='bar'></div><div class='bar'></div><div class='bar'></div><div class='bar'></div></div></div>`;
                chartContainer.html(loadingHTML);
                clubStatusAlert.textContent = '';
                eventsList.innerHTML = '';
                const [currentTides, clubStatus] = await Promise.all([fetchTideData(currentDate), fetchClubStatus(currentDate)]);
                chartContainer.html("");
                if (!currentTides || currentTides.length === 0) {
                    const message = !currentTides ? "Could not load tide data." : "No tide data available for this date.";
                    chartContainer.html(`<div style="color: red; padding: 1rem; display: flex; align-items: center; justify-content: center; height: 100%;">${message}</div>`);
                    return;
                }
                const chartData = currentTides;
                chartDataForUpdate = chartData;
                const startOfDay = d3.timeDay.floor(currentDate);
                const endOfDay = d3.timeDay.ceil(currentDate);
                const prevDay = new Date(currentDate); prevDay.setDate(prevDay.getDate() - 1);
                const nextDay = new Date(currentDate); nextDay.setDate(nextDay.getDate() + 1);
                const [prevTides, nextTides] = await Promise.all([ fetchTideData(prevDay), fetchTideData(nextDay) ]);
                const allTidesData = [...(prevTides || []), ...currentTides, ...(nextTides || [])];
                const allExtremes = findTideExtremes(allTidesData);
                const tideEvents = allExtremes.filter(event => event.time >= startOfDay && event.time < endOfDay);
                
                const widgetContainer = document.querySelector('#tide-chart-widget-container');
                if (!widgetContainer) return;
                const computedStyles = getComputedStyle(widgetContainer);
                const nightColor = computedStyles.getPropertyValue('--color-chart-night').trim();
                const dayColor = computedStyles.getPropertyValue('--color-chart-day').trim();
                const openOverlayColor = computedStyles.getPropertyValue('--color-chart-open-overlay').trim();
                const risingColor = computedStyles.getPropertyValue('--color-chart-rising').trim();
                const fallingColor = computedStyles.getPropertyValue('--color-chart-falling').trim();
                const tideLineColor = computedStyles.getPropertyValue('--color-chart-tide-line').trim();
                const redAreaColor = computedStyles.getPropertyValue('--color-chart-danger-area').trim();
                const eventCircleFill = computedStyles.getPropertyValue('--color-event-circle-fill').trim();
                const eventCircleStroke = computedStyles.getPropertyValue('--color-event-circle-stroke').trim();
                const openLineColor = computedStyles.getPropertyValue('--color-open-line').trim();
                const dockLineColor = computedStyles.getPropertyValue('--color-dock-line').trim();
                const hoverLineColor = computedStyles.getPropertyValue('--color-hover-line').trim();
                const hoverCircleFill = computedStyles.getPropertyValue('--color-hover-circle-fill').trim();
                const hoverCircleStroke = computedStyles.getPropertyValue('--color-hover-circle-stroke').trim();

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
                const { sunrise: sunriseTime, sunset: sunsetTime } = calculateSunriseSunset(currentDate, LOCATION_LAT, LOCATION_LNG);
                if (clubStatus.dockTime === 'sunset') clubStatus.dockTime = sunsetTime;
                if (clubStatus.midDayDockTime === 'sunset') clubStatus.midDayDockTime = sunsetTime;
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
                    const openPeriods = [];
                    if (clubStatus.midDayDockTime && clubStatus.midDayOpenTime) {
                        if (clubStatus.openTime && clubStatus.midDayDockTime > clubStatus.openTime) openPeriods.push({ start: clubStatus.openTime, end: clubStatus.midDayDockTime, type: 'morning' });
                        if (clubStatus.midDayOpenTime && clubStatus.dockTime > clubStatus.midDayOpenTime) openPeriods.push({ start: clubStatus.midDayOpenTime, end: clubStatus.dockTime, type: 'afternoon' });
                    } 
                    else if (clubStatus.openTime && clubStatus.dockTime > clubStatus.openTime) openPeriods.push({ start: clubStatus.openTime, end: clubStatus.dockTime, type: 'full_day' });
                    openPeriods.forEach(({start, end, type}) => {
                         svg.append("rect").attr("x", xScale(start)).attr("width", xScale(end) - xScale(start)).attr("height", height).attr("fill", openOverlayColor);
                        const openText = type === 'afternoon' ? "Club Re-Opens" : "Club Opens";
                        const dockText = type === 'morning' ? "Mid-Day Dock Time" : "Dock Time (Club Closes)";
                        if (start > startOfDay && start < endOfDay) {
                            svg.append("line").attr("x1", xScale(start)).attr("x2", xScale(start)).attr("y1", 0).attr("y2", height).attr("stroke", openLineColor).attr("stroke-width", 1.5);
                            dayEvents.push({time: start, text: openText, type: 'open'});
                        }
                        if (end > start && end < endOfDay) {
                             svg.append("line").attr("x1", xScale(end)).attr("x2", xScale(end)).attr("y1", 0).attr("y2", height).attr("stroke", dockLineColor).attr("stroke-width", 1.5);
                             dayEvents.push({time: end, text: dockText, type: 'dock'});
                        }
                    });
                }
                
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
                svg.append("path").datum(chartData).attr("fill", "none").attr("stroke", tideLineColor).attr("stroke-width", 2).attr("d", lineGenerator);
                redSegments.forEach(segment => svg.append("path").datum(segment).attr("fill", redAreaColor).attr("d", redAreaGenerator));
                svg.append("g").attr("transform", `translate(0,${height})`).call(d3.axisBottom(xScale).ticks(d3.timeHour.every(6)).tickFormat(d3.timeFormat("%H:%M"))).attr("class", "axis x-axis");
                const yAxis = d3.axisLeft(yScale), ticks = yScale.ticks(), uniqueRoundedTicks = [...new Set(ticks.map(t => Math.round(t)))];
                yAxis.tickValues(uniqueRoundedTicks).tickFormat(d => `${d} ft`);
                svg.append("g").call(yAxis).attr("class", "axis y-axis");
                const eventsGroup = svg.selectAll(".event").data(tideEvents).enter().append("g").attr("class", "event").attr("transform", d => `translate(${xScale(d.time)},${yScale(d.height)})`);
                eventsGroup.append("circle").attr("r", 5).attr("fill", eventCircleFill).attr("stroke", eventCircleStroke).attr("stroke-width", 1.5);
                eventsGroup.each(function(d) {
                    d3.select(this).append("text").attr("class", "event-label").attr("dy", "-10px").attr("text-anchor", "middle").text(`${d.height.toFixed(2)}ft`);
                });
                updateNowIndicator();
                eventsGroup.on("mouseover", (event, d) => { isHoveringOnEvent = true; tooltip.style("opacity", 1).style("left", `${event.pageX + 15}px`).style("top", `${event.pageY - 15}px`); tooltip.select("#tooltip-time").text(`Event: ${d3.timeFormat("%H:%M")(d.time)}`); tooltip.select("#tooltip-height").text(`${d.height.toFixed(2)} ft`); }).on("mouseout", () => { isHoveringOnEvent = false; tooltip.style("opacity", 0); });
                const hoverGroup = svg.append("g").attr("class", "hover-group").style("display", "none");
                hoverGroup.append("line").attr("stroke", hoverLineColor).attr("stroke-width", 1).attr("y1", 0).attr("y2", height);
                const hoverCircle = hoverGroup.append("circle").attr("r", 5).attr("fill", hoverCircleFill).attr("stroke", hoverCircleStroke).attr("stroke-width", 1.5);
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
                    .on("mouseover", () => { if (!isHoveringOnEvent) { hoverGroup.style("display", null); tooltip.style("opacity", 1); }})
                    .on("mouseout", () => { hoverGroup.style("display", "none"); if (!isHoveringOnEvent) tooltip.style("opacity", 0); })
                    .on("mousemove", handleInteractionMove)
                    .on("touchstart", (e) => { e.preventDefault(); if (!isHoveringOnEvent) { hoverGroup.style("display", null); tooltip.style("opacity", 1); }}, { passive: false })
                    .on("touchmove", handleInteractionMove, { passive: false })
                    .on("touchend", () => { hoverGroup.style("display", "none"); if (!isHoveringOnEvent) tooltip.style("opacity", 0);})
                    .on("touchcancel", () => { hoverGroup.style("display", "none"); if (!isHoveringOnEvent) tooltip.style("opacity", 0);});
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
                          open: `<svg class="icon-sm icon-open" fill="none" viewBox="0 0 24 24" stroke="currentColor"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M14.752 11.168l-3.197-2.132A1 1 0 0010 9.87v4.263a1 1 0 001.555.832l3.197-2.132a1 1 0 000-1.664z"></path><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M21 12a9 9 0 11-18 0 9 9 0 0118 0z"></path></svg>`,
                          dock: `<svg class="icon-sm icon-dock" fill="none" viewBox="0 0 24 24" stroke="currentColor"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M10 9v6m4-6v6m7-3a9 9 0 11-18 0 9 9 0 0118 0z"></path></svg>`,
                          high: `<svg class="icon-sm icon-high" fill="none" viewBox="0 0 24 24" stroke="currentColor"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M5 10l7-7m0 0l7 7m-7-7v18"></path></svg>`,
                          low: `<svg class="icon-sm icon-low" fill="none" viewBox="0 0 24 24" stroke="currentColor"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M19 14l-7 7m0 0l-7-7m7 7V3"></path></svg>`,
                          sunset: `<svg class="icon-sm icon-sunset" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" viewBox="0 0 24 24"><path d="M17 18a5 5 0 00-10 0M12 2v7m-5.66 2.34l1.42 1.42M1 18h2m18 0h2M18.36 11.64l1.42-1.42M23 22H1m15-17l-4 4-4-4"/></svg>`
                     };
                     let tableHTML = `<table><tbody>${uniqueEvents.map(e => { const icon = eventIcons[e.type] || ''; return `<tr><td>${icon}</td><td>${e.text}</td><td><strong>${d3.timeFormat("%H:%M")(new Date(e.time))}</strong></td></tr>`; }).join('')}</tbody></table>`;
                     eventsList.innerHTML = tableHTML;
                } else {
                     eventsList.innerHTML = `<p>No scheduled events during daylight hours.</p>`;
                }
                updateUI();
            }

            function updateNowIndicator() {
                if (!svg || !xScale || !yScale || !chartDataForUpdate) return;
                svg.select(".now-indicator").remove();
                const now = new Date();
                const startOfDay = d3.timeDay.floor(currentDate);
                const endOfDay = d3.timeDay.ceil(currentDate);
                if (currentDate.toDateString() === now.toDateString() && now >= startOfDay && now <= endOfDay) {
                    const bisect = d3.bisector(d => d.time).left;
                    const index = bisect(chartDataForUpdate, now, 1);
                    const a = chartDataForUpdate[index - 1], b = chartDataForUpdate[index];
                    const currentDataPoint = !a ? b : !b ? a : (now - a.time > b.time - now) ? b : a;
                    if (currentDataPoint) {
                         const widgetContainer = document.querySelector('#tide-chart-widget-container');
                         if (!widgetContainer) return;
                         const computedStyles = getComputedStyle(widgetContainer);
                         const nowIndicatorColor = computedStyles.getPropertyValue('--color-now-indicator').trim();
                         svg.append("circle").attr("class", "now-indicator").attr("cx", xScale(now)).attr("cy", yScale(currentDataPoint.height)).attr("r", 5).attr("fill", nowIndicatorColor).style("pointer-events", "none");
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
                document.getElementById('legend').innerHTML = `<div><div style="width: 0.75rem; height: 0.75rem; border-radius: 0.125rem; background-color: var(--color-chart-open-overlay);"></div><span>Club Open</span></div><div><div style="width: 2px; height: 0.75rem; background-color: var(--color-open-line)"></div><span>Open Time</span></div><div><div style="width: 2px; height: 0.75rem; background-color: var(--color-dock-line)"></div><span>Dock Time</span></div><div><div style="width: 0.75rem; height: 0.75rem; border-radius: 9999px; background-color: var(--color-now-indicator);"></div><span>Now</span></div><div><div style="width: 0.75rem; height: 0.75rem; background-color: var(--color-chart-danger-area);"></div><span>Club Closed < 1.5' Tide</span></div>`;
            }

            async function updateNextTideEvent() {
                 try {
                    const now = new Date();
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
                            if (futureHeight > currentHeight) directionArrow = `<svg class="icon-sm" style="display: inline-block; vertical-align: middle; color: var(--icon-color-high);" fill="none" viewBox="0 0 24 24" stroke="currentColor" stroke-width="2"><path stroke-linecap="round" stroke-linejoin="round" d="M5 10l7-7m0 0l7 7m-7-7v18"></path></svg>`;
                            else if (futureHeight < currentHeight) directionArrow = `<svg class="icon-sm" style="display: inline-block; vertical-align: middle; color: var(--icon-color-low);" fill="none" viewBox="0 0 24 24" stroke="currentColor" stroke-width="2"><path stroke-linecap="round" stroke-linejoin="round" d="M19 14l-7 7m0 0l-7-7m7 7V3"></path></svg>`;
                        }
                        currentTideStatus.innerHTML = `<span style="display: inline-flex; align-items: center;">${directionArrow}<span style="margin-left: 0.25rem;">${currentHeight.toFixed(2)} ft</span></span>`;
                    } else {
                        currentTideStatus.innerHTML = "";
                    }
                    const allRawTides = [...prevTides, ...currentTides, ...nextTides, ...nextNextTides];
                    const allTideExtremes = findTideExtremes(allRawTides);
                    const nextEvent = allTideExtremes.find(event => event.time > now);
                    if (nextEvent) {
                        const diff = nextEvent.time - now, hours = Math.floor(diff / 3600000), minutes = Math.floor((diff % 3600000) / 60000);
                        nextTideDisplay.innerHTML = `<span>${nextEvent.type} tide in ${hours}h ${minutes}m at ${nextEvent.height.toFixed(2)} ft</span>`;
                    } else {
                        nextTideDisplay.innerHTML = "";
                    }
                    updateNowIndicator();
                } catch (error) {
                    console.error("Error during periodic tide update:", error);
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
