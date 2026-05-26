import { Button } from "@/components/ui/button"
import {
  Dialog,
  DialogClose,
  DialogContent,
  DialogDescription,
  DialogFooter,
  DialogHeader,
  DialogTitle,
  DialogTrigger,
} from "@/components/ui/dialog"
import { Slider } from "@/components/ui/slider"
import { Input } from "@/components/ui/input"
import { SettingsIcon } from "lucide-react"
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs"
import ModuleCard from "../ModuleCard"


export default function NeopixelSettings() {

  return (
    <Dialog>
        <DialogTrigger><ModuleCard name="Neopixel" /></DialogTrigger>
        <DialogContent>


            <DialogHeader>
                <DialogTitle>"Left Arch" Neopixel [1] Settings</DialogTitle>
                <DialogDescription>
                    Module slot 1 on the "Left Arch" controller is configured as a Neopixel module.
                </DialogDescription>
            </DialogHeader>


            <p className="text-md font-semibold -mb-3 mt-4">Select Output</p>
            <Tabs defaultValue="1">
                <TabsList className="w-full">
                    <TabsTrigger value="1">1</TabsTrigger>
                    <TabsTrigger value="2">2</TabsTrigger>
                    <TabsTrigger value="3">3</TabsTrigger>
                    <TabsTrigger value="4">4</TabsTrigger>
                </TabsList>
                <TabsContent value="1">

                    <p className="text-md font-semibold mb-1 mt-4">Pixel Count</p>
                    <Input placeholder="123" />

                    <p className="text-md font-semibold mb-2 mt-6">Max Brightness</p>
                    <Slider defaultValue={[100]} max={100} step={1} />

                    <p className="text-md font-semibold mb-1 mt-6">Protocol</p>
                    <Tabs defaultValue="WS2812">
                        <TabsList className="w-full">
                            <TabsTrigger value="WS2812">WS2812</TabsTrigger>
                            <TabsTrigger value="WS2811">WS2811</TabsTrigger>
                            <TabsTrigger value="SK6812">SK6812</TabsTrigger>
                            <TabsTrigger value="APA102">APA102</TabsTrigger>
                        </TabsList>
                    </Tabs>

                </TabsContent>
            </Tabs>


            
           



            <DialogFooter>
                <DialogClose asChild>
                    <Button variant="outline">Back</Button>
                </DialogClose>
                <Button type="submit">Ok</Button>
            </DialogFooter>



        </DialogContent>
    </Dialog>
  )
}