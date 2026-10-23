idf_component_register(
    SRCS "src/4deiacs.cpp"
        "src/core/4deiacs_node_engine.cpp"
        "src/core/management/block_registry.cpp"
        "src/blocks/timers/e_cycle_block.cpp"
        "src/blocks/io/serial_monitor.cpp"
    INCLUDE_DIRS "include"
                "include/core"
                "include/core/management"
                "include/blocks/timers"
                "include/blocks/io"
    REQUIRES
        cjson
        esp_event
        esp_timer
    WHOLE_ARCHIVE
)