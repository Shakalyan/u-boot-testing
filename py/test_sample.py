import pytest

def f(x):
    return x + 1

#@pytest.mark.boardspec('sandbox')
#@pytest.mark.buildconfigspec('cmd_bind')
def test_f(u_boot_console):
    assert f(5) == 5