from concurrence import dispatch, Channel

# Do nothing
def main():
    pass

# Run main loop (called from StacklessThread)
def run():
    dispatch(main)
