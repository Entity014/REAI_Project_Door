import discord
import qrcode
from discord.ext import commands

intents = discord.Intents.default()
intents.message_content = True
bot = commands.Bot(command_prefix='>', intents=intents)

@bot.command()
async def test(ctx):
    # if (ctx.author == '_Entity014#5176'):
    await ctx.send('You are master.', file=discord.File('Master.png'))

@bot.command()
@commands.has_role("Master")
async def QrM(ctx):
    await ctx.message.author.send('You are master.', file=discord.File('REAI-Pro\image\Master.png'))

@bot.command()
@commands.has_role("Slave")
async def QrS(ctx):
    await ctx.message.author.send('You are slave.', file=discord.File('REAI-Pro\image\Slave.png'))

bot.run('NTQ0ODM0ODc1MzE2ODk1NzQ1.GEWjtT.PdJBErIKWKPoN-BPD0ltMZjePmtrpwL2SwvPVs')
